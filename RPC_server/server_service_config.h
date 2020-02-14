#ifndef CLIENT_SERVICE_CONFIG_H
#define CLIENT_SERVICE_CONFIG_H
#include "config_read.h"
#include <string>

class ServerServiceConfig
{
public:
	ServerServiceConfig(const char* path);
	~ServerServiceConfig();
	std::string ip()const;
	int port()const;
	int max_client_count()const;
	int worker_count()const;
	int epoll_workers()const;
	int cache_size()const;
private:
	Config m_config;
};

#endif	//CLIENT_SERVICE_CONFIG_H
