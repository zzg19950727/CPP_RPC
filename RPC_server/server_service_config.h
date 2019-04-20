#ifndef CLIENT_SERVICE_CONFIG_H
#define CLIENT_SERVICE_CONFIG_H

class ServerServiceConfig
{
public:
	ServerServiceConfig(const char* path);
	~ServerServiceConfig();
	const char* ip()const;
	int port()const;
	int max_client_count()const;
	int worker_count()const;
	int cache_size()const;
};

#endif	//CLIENT_SERVICE_CONFIG_H
