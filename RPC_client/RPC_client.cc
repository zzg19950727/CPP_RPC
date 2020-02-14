#include <google/protobuf/io/coded_stream.h>
#include "RPC_template.pb.h"
#include "RPC_client_service.h"
#include "RPC_client.h"
#include "log_utils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <functional>
#include <exception>
#include <iostream>
#include <chrono>
#include <cstring>
#define BLOCK_SIZE	2048

ClientServiceConfig g_service_config("RPC_client.conf");
ClientService g_client_service(g_service_config);

size_t send_request(const std::string& method, int query_id, unsigned char* buf, size_t len)
{
	return g_client_service.send_request(method,query_id, (char*)buf,len);
}

std::string const error_string(State s)
{
	switch(s)
	{
	case S_DONE:
		return "request done";
	case S_TIME_OUT:
		return "request time out";
	case S_LOCAL_CACHE_FULL:
		return "local cache full,reject request";
	case S_SERVER_ERROR:
		return "RPC server error";
	case S_NO_CONNECTION:
		return "RPC server disconnect";
	}
}

int connect_to_server(const std::string& ip, int port)
{
    int fd;
	fd = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	server_addr.sin_port = htons(port);

    if(connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1)
	{
		log(LOG_USER|LOG_ERR, "RPC client service connect to %s:%d error:%d", ip.c_str(), port, errno);
        return -1;
	}
    else
	{
		log(LOG_USER|LOG_DEBUG, "RPC client service connect to %s:%d success", ip.c_str(), port);
        return fd;
	}
}

int response_tag(char* buf, size_t len)
{
	RPC::RPC_response_template response;
	::google::protobuf::io::CodedInputStream input((unsigned char*)buf,len);
	response.MergePartialFromCodedStream(&input);
	std::string func = response.method();
	int query_id = response.query_id();
	int tag = ClientService::hash(func,query_id);;

	log(LOG_USER|LOG_DEBUG, "ClientService recv response %s,len:%zu,tag:%d",func.c_str(),len,tag);
	return tag;
}

ClientService::ClientService(ClientServiceConfig& config)
	:m_read_cache(config.cache_size()),
	m_write_cache(config.cache_size()),
	m_config(config),
	m_io_service(config.max_server_count())
{
	m_fd = connect_to_server(config.ip(),config.port());
	if(m_fd > 0)
	{
		IoService::CallbackFunc func = std::bind(&ClientService::notify_socket,this, std::placeholders::_1, std::placeholders::_2);
		if(m_io_service.register_io(m_fd, IoService::E_RW, func) < 0)
		{
			close_connection();
		}
		else
		{
			m_io_service.modify_io_event(m_fd, IoService::E_READ);
		}
	}
}

ClientService::~ClientService()
{
	close_connection();
}

size_t ClientService::send_request(const std::string& method, int query_id, char* buf, size_t len)
{
	log(LOG_USER|LOG_DEBUG, "ClientService::send_request %s:%d", method.c_str(), query_id);
	if(m_fd < 0)
	{
		log(LOG_USER|LOG_ERR, "ClientService::send_request %s:%d no connect to server", method.c_str(), query_id);
		throw std::runtime_error(error_string(S_NO_CONNECTION));
	}
	int tag = hash(method, query_id);
	
	//create response handle for request
	auto handle = std::make_shared<ResponseHandle>(m_condition, m_timer_manager, tag);
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_response_handles[tag] = handle;
	}
	while(1)
	{
		m_write_cache.write_package(buf,len);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		m_io_service.modify_io_event(m_fd, IoService::E_RW);
	}
	//write request to cache
	if(!m_write_cache.write_package(buf,len))
	{
		log(LOG_USER|LOG_ERR, "ClientService::send_request write_cache full,rejuect request %s:%d", method.c_str(), query_id);
		throw std::runtime_error(error_string(S_LOCAL_CACHE_FULL));
	}
	else
	{
		m_io_service.modify_io_event(m_fd, IoService::E_RW);
	}

	log(LOG_USER|LOG_DEBUG, "ClientService::send_request %s:%d wait for response,tag %d", method.c_str(), query_id, tag);
	len = handle->read(buf, BLOCK_SIZE, m_config.request_timeout());

	if(len == 0)
	{
		log(LOG_USER|LOG_ERR, "ClientService::send_request request  %s:%d error:%s",method.c_str(), query_id,error_string(S_TIME_OUT).c_str());
		throw std::runtime_error(error_string(S_TIME_OUT));
	}
	
	log(LOG_USER|LOG_DEBUG, "ClientService::send_request request %s:%d done", method.c_str(), query_id);
	return len;
}

int ClientService::hash(const std::string& method, int query_id)
{
	std::hash<std::string> str_hash;
	return query_id^str_hash(method);
}

void ClientService::notify_socket(int fd, IoService::Event e)
{
	if(e == IoService::E_READ)
		read_socket(fd);
	else if(e == IoService::E_WRITE)
		write_socket(fd);
}

void ClientService::read_socket(int fd)
{
	Buffer buffer;
	buffer_calloc(buffer, BLOCK_SIZE);
	size_t len = BLOCK_SIZE;
	len = read(fd, buffer.data, len);
	log(LOG_USER|LOG_DEBUG, "ClientService::read_socket recv %zu bytes data",len);

	buffer.length = len;
	m_read_cache.write(buffer);
	
   	Header head;
	while(m_read_cache.read_package(head,buffer))
	{
		log(LOG_USER|LOG_DEBUG, "ClientService::read_socket read package");
		int tag = response_tag(buffer.data, buffer.length);
		std::shared_ptr<ResponseHandle> handle;
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			auto iter = m_response_handles.find(tag);
			if( iter != m_response_handles.cend())
			{
				handle = m_response_handles[tag];
				m_response_handles.erase(iter);
			}
		}
		if(handle)
		{
			handle->write(buffer.data, buffer.length);
		}
	}
	
	//disconnect
	if(len == 0)
	{
		close_connection();
	}
	buffer_free(buffer);
}

void ClientService::write_socket(int fd)
{
	Buffer tmp;
	buffer_calloc(tmp, BLOCK_SIZE);
	
	while(true)
	{
		tmp.length = BLOCK_SIZE;
		m_write_cache.have_a_look(tmp);
		size_t len = tmp.length;
		len = write(fd, tmp.data, len);

		tmp.length = len;
		if(len > 0)
		{
			m_write_cache.read(tmp);
			log(LOG_USER|LOG_DEBUG, "ClientService::write_socket send %zu bytes data",len);
		}
		if(len != BLOCK_SIZE)
			break;
	}
	m_io_service.modify_io_event(m_fd, IoService::E_READ);
	buffer_free(tmp);
}

void ClientService::close_connection()
{
	m_io_service.unregister_io(m_fd, IoService::E_RW);
	close(m_fd);
	m_fd = -1;
}
