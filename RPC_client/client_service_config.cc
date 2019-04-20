#include "client_service_config.h"
#include "log_utils.h"

ClientServiceConfig::ClientServiceConfig(const char* path)
{
	openlog("RPC_client", LOG_CONS | LOG_PID, 0);
}

ClientServiceConfig::~ClientServiceConfig()
{
	closelog();
}

const char* ClientServiceConfig::ip()const
{
	return "127.0.0.1";
}

int ClientServiceConfig::port()const
{
	return 1234;
}

int ClientServiceConfig::max_server_count()const
{
	return 1;
}

int ClientServiceConfig::request_timeout()const
{
	return 2000;
}

int ClientServiceConfig::cache_size()const
{
	return 1024000;
}
