#include "client_service_config.h"
#include "log_utils.h"

ClientServiceConfig::ClientServiceConfig(const char* path)
	:m_config(path)
{
	openlog("RPC_client", LOG_CONS | LOG_PID, 0);
}

ClientServiceConfig::~ClientServiceConfig()
{
	closelog();
}

std::string ClientServiceConfig::ip()const
{
	std::string ip = m_config.value("ip");
	if(ip.empty())
		return "127.0.0.1";
	else
		return ip;
}

int ClientServiceConfig::port()const
{
	std::string port = m_config.value("port");
	if(port.empty() || std::stoi(port)<=0 )
		return 1234;
	else
		return std::stoi(port);
}

int ClientServiceConfig::max_server_count()const
{
	std::string count = m_config.value("max_server_count");
	if(count.empty() || std::stoi(count)<=0)
		return 1;
	else
		return std::stoi(count);
}

int ClientServiceConfig::request_timeout()const
{
	std::string timeout = m_config.value("request_timeout");
	if(timeout.empty() || std::stoi(timeout)<=0)
		return 3000;
	else
		return std::stoi(timeout);
}

int ClientServiceConfig::cache_size()const
{
	std::string size = m_config.value("cache_size");
	if(size.empty() || std::stoi(size)<=0)
		return 1024000;
	else
		return std::stoi(size);
}
