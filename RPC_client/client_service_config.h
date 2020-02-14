#ifndef CLIENT_SERVICE_CONFIG_H
#define CLIENT_SERVICE_CONFIG_H
#include "config_read.h"
#include <string>

class ClientServiceConfig
{
public:
	ClientServiceConfig(const char* path);
	~ClientServiceConfig();
	std::string ip()const;
	int port()const;
	int max_server_count()const;
	int request_timeout()const;
	int cache_size()const;
private:
	Config m_config;
};

#endif	//CLIENT_SERVICE_CONFIG_H
