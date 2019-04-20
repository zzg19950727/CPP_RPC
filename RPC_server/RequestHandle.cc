#include <google/protobuf/io/coded_stream.h>
#include "RPC_server_service.h"
#include "RPC_template.pb.h"
#include "RequestHandle.h"
#include "log_utils.h"
#include <unistd.h>
#define BLOCK_SIZE	2048

RequestHandle::RequestHandle(int fd,ServerService& service)
	:m_fd(fd),
	m_read_cache(service.config().cache_size()),
	m_write_cache(service.config().cache_size()),
	m_server_service(service)
{
	IoService::CallbackFunc func = std::bind(&RequestHandle::notify_socket,this, std::placeholders::_1, std::placeholders::_2);
	if(m_server_service.io_service().register_io(m_fd, IoService::E_RW, func) < 0)
	{
		close_connection();
	}
}

RequestHandle::~RequestHandle()
{
	if(m_fd > 0)
		close_connection();
}

void RequestHandle::set_delete_handle(std::shared_ptr<RequestHandle>& self)
{
	m_self.swap(self);
}

void RequestHandle::notify_socket(int fd, IoService::Event e)
{
	if(e == IoService::E_READ)
		read_socket(fd);
	else if(e == IoService::E_WRITE)
		write_socket(fd);
}

void RequestHandle::read_socket(int fd)
{
	char data[BLOCK_SIZE];
	size_t len = BLOCK_SIZE;
	len = read(fd, data, len);
	
	m_server_service.monitor().recv_bytes(len);
	log(LOG_USER|LOG_DEBUG, "RequestHandle::read_socket fd %d recv %zu bytes data",m_fd,len);

	Buffer buffer;
	buffer.capacity = BLOCK_SIZE;
	buffer.length = len;
	buffer.data = data;
	m_read_cache.write(buffer);
	
	Header head;
	bool state = true;
	while(state)
	{
		buffer.length = BLOCK_SIZE;
		if(state=m_read_cache.read_package(head,buffer))
		{
			handle_request((unsigned char*)buffer.data, buffer.length);
		}
	}
	//disconnect
	if(len == 0)
	{
		close_connection();
	}
}

void RequestHandle::write_socket(int fd)
{
	char buf[BLOCK_SIZE];
	Buffer tmp;
	tmp.capacity = BLOCK_SIZE;
	tmp.length = BLOCK_SIZE;
	tmp.data = buf;

	m_write_cache.have_a_look(tmp);
	size_t len = tmp.length;
	len = write(fd, buf, len);

	tmp.length = len;
	if(len > 0)
	{
		m_write_cache.read(tmp);
		m_server_service.monitor().send_bytes(len);
		log(LOG_USER|LOG_DEBUG, "RequestHandle::write_socket fd %d send %zu bytes data",m_fd,len);
	}
}

void RequestHandle::close_connection()
{
	log(LOG_USER|LOG_DEBUG, "RequestHandle close connection:%d",m_fd);
	m_server_service.io_service().unregister_io(m_fd, IoService::E_RW);
	m_server_service.close_connection(m_fd);
	std::shared_ptr<RequestHandle> copy;
	copy.swap(m_self);
	close(m_fd);
	m_fd = -1;
}

template <typename T>
std::shared_ptr<T> make_shared_array(size_t size)
{
    return std::shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

void RequestHandle::worker_caller(const std::string& func, std::shared_ptr<char> ptr, size_t len)
{
	log(LOG_USER|LOG_DEBUG, "RequestHandle::worker_caller call func %s( 0x%p, %zu)", func.c_str(),ptr.get(), len);
	len = m_server_service.rpc_funcs()[func]((unsigned char*)ptr.get(),len);
	if(!m_write_cache.write_package(ptr.get(),len))
	{
		log(LOG_USER|LOG_ERR, "RequestHandle write_cache full,drop %s`s response", func.c_str());
	}
	else
	{
		m_server_service.monitor().send_responses(1);
	}
}

void RequestHandle::handle_request(unsigned char* buf, size_t len)
{
	RPC::RPC_request_template request;
	::google::protobuf::io::CodedInputStream input(buf,len);
	request.MergePartialFromCodedStream(&input);
	std::string func = request.method();
	int id = request.query_id();

	m_server_service.monitor().recv_requests(1);
	log(LOG_USER|LOG_DEBUG, "RequestHandle::handle_request %s:%d", func.c_str(), id);
	if(!m_server_service.rpc_funcs().count(func))
	{
		log(LOG_USER|LOG_WARNING, "RequestHandle::handle_request no handle for %s:%d",func.c_str(),id);
		return;
	}
	
	auto ptr = make_shared_array<char>(len);
	memcpy(ptr.get(),buf,len);
	
	RPC::Task_type task = std::bind(&RequestHandle::worker_caller,this,
		func,ptr,len);
	
	m_server_service.workers().append_task(task);
}
