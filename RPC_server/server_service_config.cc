#include "server_service_config.h"
#include "log_utils.h"

ServerServiceConfig::ServerServiceConfig(const char* path)
	:m_config(path)
{
	openlog("RPC_server", LOG_CONS | LOG_PID, 0);
}

ServerServiceConfig::~ServerServiceConfig()
{
	closelog();
}

std::string ServerServiceConfig::ip()const
{
	std::string ip = m_config.value("ip");
	if(ip.empty())
		return "127.0.0.1";
	else
		return ip;
}

int ServerServiceConfig::port()const
{
	std::string port = m_config.value("port");
	if(port.empty() || std::stoi(port)<=0 )
		return 1234;
	else
		return std::stoi(port);
}

int ServerServiceConfig::max_client_count()const
{
	std::string count = m_config.value("max_client_count");
	if(count.empty() || std::stoi(count)<=0)
		return 2048;
	else
		return std::stoi(count);
}

int ServerServiceConfig::cache_size()const
{
	std::string size = m_config.value("cache_size");
	if(size.empty() || std::stoi(size)<=0)
		return 1024000;
	else
		return std::stoi(size);
}

int ServerServiceConfig::worker_count()const
{
	std::string count = m_config.value("worker_count");
	if(count.empty() || std::stoi(count)<=0)
		return 600;
	else
		return std::stoi(count);
}

int ServerServiceConfig::epoll_workers()const
{
	std::string count = m_config.value("epoll_workers");
	if(count.empty() || std::stoi(count)<=0)
		return 40;
	else
		return std::stoi(count);
}
