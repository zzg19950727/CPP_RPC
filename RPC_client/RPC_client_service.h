#ifndef RPC_CLIENT_SERVICE_H
#define RPC_CLIENT_SERVICE_H
#include "client_service_config.h"
#include "ResponseHandle.h"
#include "TimerManager.h"
#include "ThreadPool.hpp"
#include "IoService.h"
#include "buffer.h"
#include <string>
#include <memory>
#include <mutex>
#include <map>

class ClientService
{
public:
	ClientService(ClientServiceConfig& config);
	~ClientService();
	
	size_t send_request(const std::string& method, int query_id, char* buf, size_t len);
	static int hash(const std::string& method, int query_id);
private:
	void notify_socket(int fd, IoService::Event e);
	void read_socket(int fd);
	void write_socket(int fd);
	void close_connection();
	
	BufferCache m_read_cache;
	BufferCache m_write_cache;
	TimerManager m_timer_manager;
	ClientServiceConfig& m_config;
	IoService m_io_service;
	RPC::Condition m_condition;
	int m_fd;

	std::map<int,std::shared_ptr<ResponseHandle>> m_response_handles;
	std::mutex m_mutex;
};


#endif	//RPC_CLIENT_SERVICE_H
