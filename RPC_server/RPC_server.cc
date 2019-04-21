#include "RPC_server_service.h"
#include "RequestHandle.h"
#include "RPC_server.h"
#include "log_utils.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

ServerServiceConfig g_server_config(0);
ServerService g_server_service(g_server_config);

void run()
{
	g_server_service.start_monite();
}

RegisHelper::RegisHelper(const std::string& func_name, RPC_func_type func)
{
	log(LOG_USER|LOG_DEBUG, "RegisHelper register RPC:%s",func_name.c_str());
	ServerService::RPC_func_type rpc_func(func);
	g_server_service.register_rpc_func(func_name,rpc_func);
}

int start_server(int port, int listen_n)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		log(LOG_USER|LOG_ERR, "ServerService create socket failed");
		return -1;
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	if(bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		log(LOG_USER|LOG_ERR, "ServerService bind address failed");
		return -2;
	}
    
	if(listen(fd, listen_n) < 0)
	{
		log(LOG_USER|LOG_ERR, "ServerService listen failed");
		return -3;
	}
	log(LOG_USER|LOG_DEBUG, "start ServerService success");
	return fd;
}

int accept_connection(int fd)
{
	socklen_t addrlen = sizeof(struct sockaddr);
	struct sockaddr_in client_addr;
	int client_fd = accept(fd, (struct sockaddr*)&client_addr, &addrlen);
	
	log(LOG_USER|LOG_DEBUG, "accept connection:%d",client_fd);
	return client_fd;
}

ServerService::ServerService(ServerServiceConfig& config)
	:m_io_service(config.max_client_count()),
	m_workers(config.worker_count()),
	m_config(config),
	m_clients(0)
{
	m_fd = start_server(config.port(), config.max_client_count());
	if(m_fd > 0)
	{
		IoService::CallbackFunc func = std::bind(&ServerService::new_connection,this, std::placeholders::_1, std::placeholders::_2);
		if(m_io_service.register_io(m_fd, IoService::E_RW, func) < 0)
		{
			close_connection();
		}
	}
}

ServerService::~ServerService()
{
	close_connection();
}

void ServerService::start_monite()
{
	m_monitor.monite(2000);
}

void ServerService::register_rpc_func(const std::string& func_name, RPC_func_type& func)
{
	if(m_rpc_funcs.count(func_name))
		log(LOG_USER|LOG_WARNING, "ServerService::register_rpc_func %s has registered",func_name.c_str());
	m_rpc_funcs[func_name] = func;
}

void ServerService::unregister_rpc_func(const std::string& func_name)
{
	if(!m_rpc_funcs.count(func_name))
		log(LOG_USER|LOG_WARNING, "ServerService::unregister_rpc_func %s has not registered",func_name.c_str());
	m_rpc_funcs.erase(func_name);
}
	
void ServerService::new_connection(int fd, IoService::Event e)
{
	int client_fd = accept_connection(fd);
	if(client_fd > 0)
	{
		if(m_clients >= m_config.max_client_count())
		{
			log(LOG_USER|LOG_WARNING, "ServerService::new_connection too much clients,rejuect");
			close(client_fd);
			return;
		}
		auto ptr = std::make_shared<RequestHandle>(client_fd, *this);
		ptr->set_delete_handle(ptr);
		++m_clients;
		m_monitor.add_connections(1);
	}
}

void ServerService::close_connection(int fd)
{
	--m_clients;
	m_monitor.remove_connections(1);
}

void ServerService::close_connection()
{
	log(LOG_USER|LOG_DEBUG, "stop ServerService success");
	close(m_fd);
	m_fd = -1;
}

IoService& ServerService::io_service()
{
	return m_io_service;
}

RPC::ThreadPool& ServerService::workers()
{
	return m_workers;
}

ServerService::RPC_func_map& ServerService::rpc_funcs()
{
	return m_rpc_funcs;
}

ServerServiceConfig& ServerService::config()
{
	return m_config;
}

ServerMonitor& ServerService::monitor()
{
	return m_monitor;
}
