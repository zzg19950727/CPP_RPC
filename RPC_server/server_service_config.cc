#include "server_service_config.h"
#include "log_utils.h"

ServerServiceConfig::ServerServiceConfig(const char* path)
{
	openlog("RPC_server", LOG_CONS | LOG_PID, 0);
}

ServerServiceConfig::~ServerServiceConfig()
{
	closelog();
}

const char* ServerServiceConfig::ip()const
{
	return "127.0.0.1";
}

int ServerServiceConfig::port()const
{
	return 1234;
}

int ServerServiceConfig::max_client_count()const
{
	return 100;
}

int ServerServiceConfig::cache_size()const
{
	return 1024000;
}

int ServerServiceConfig::worker_count()const
{
	return 20;
}
