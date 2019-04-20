#ifndef CLIENT_SERVICE_CONFIG_H
#define CLIENT_SERVICE_CONFIG_H

class ClientServiceConfig
{
public:
	ClientServiceConfig(const char* path);
	~ClientServiceConfig();
	const char* ip()const;
	int port()const;
	int max_server_count()const;
	int request_timeout()const;
	int cache_size()const;
};

#endif	//CLIENT_SERVICE_CONFIG_H
