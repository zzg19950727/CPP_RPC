#ifndef RPC_SERVER_SERVICE_H
#define RPC_SERVER_SERVICE_H
#include "server_service_config.h"
#include "ServerMonitor.h"
#include "ThreadPool.hpp"
#include "IoService.h"
#include <functional>
#include <string>
#include <map>

class RequestHandle;
class ServerService
{
public:
	typedef std::function<size_t(unsigned char*,size_t)> RPC_func_type;
	typedef std::map<std::string,RPC_func_type>	RPC_func_map;
	ServerService(ServerServiceConfig& config);
	~ServerService();
	void start_monite();
	void register_rpc_func(const std::string& func_name, RPC_func_type& func);
	void unregister_rpc_func(const std::string& func_name);
	
private:
	void new_connection(int fd, IoService::Event e);
	void close_connection(int fd);
	void close_connection();
	
	IoService& io_service();
	RPC::ThreadPool& workers();
	RPC_func_map& rpc_funcs();
	ServerServiceConfig& config();
	ServerMonitor& monitor();
	
	IoService m_io_service;
	RPC::ThreadPool m_workers;
	RPC_func_map m_rpc_funcs;
	ServerServiceConfig& m_config;
	ServerMonitor m_monitor;
	
	int m_fd;
	int m_clients;

friend class RequestHandle;
};

#endif	//RPC_SERVER_SERVICE_H

