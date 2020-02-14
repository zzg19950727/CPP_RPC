#include "ServerMonitor.h"
#include "TimerManager.h"
#include <chrono>
#include <string>
#include <stdio.h>

void clrsr()
{
	printf("\x1b[H\x1b[2J");
}
void gotoxy(int x,int y)
{ 
   printf("%c[%d;%df",0x1B,y,x); 
}

std::string to_string(float f)
{
	std::string n = std::to_string(f);
	int end = n.size();
	for(int i=0; i<n.size(); ++i)
	{
		if(n[i]=='.')
		{
			end = i+2;
		}
	}
	return std::string(n,0,end>n.size()?n.size():end);
}

std::string bytes2str(long long byte)
{
	float bytes = byte;
	if(bytes < 1024)
	{
		return to_string(bytes)+"B";
	}
	
	bytes /= 1024.0;
	if(bytes < 1024)
	{
		return to_string(bytes)+"KB";
	}

	bytes /= 1024.0;
	if(bytes < 1024)
	{
		return to_string(bytes)+"MB";
	}

	bytes /= 1024.0;
	if(bytes < 1024)
	{
		return to_string(bytes)+"GB";
	}

	bytes /= 1024.0;
	return to_string(bytes)+"TB";
} 

std::string speed(long long bytes, long long msec)
{
	std::string s = bytes2str(bytes*1000/msec);
	s += "/s";
	return s;
}

ServerMonitor::ServerMonitor()
	:m_exit(false),
	m_connections(0),
	m_recv_bytes(0),
	m_send_bytes(0),
	m_recv_requests(0),
	m_send_responses(0)
{
}

ServerMonitor::~ServerMonitor()
{
	m_exit = true;
}

void ServerMonitor::add_connections(int n)
{
	m_connections += n;
}

void ServerMonitor::remove_connections(int n)
{
	m_connections -= n;
}

void ServerMonitor::recv_bytes(int n)
{
	m_recv_bytes += n;
}

void ServerMonitor::send_bytes(int n)
{
	m_send_bytes += n;
}
void ServerMonitor::recv_requests(int n)
{
	m_recv_requests += n;
}

void ServerMonitor::send_responses(int n)
{
	m_send_responses += n;
}

void ServerMonitor::set_task_queue_size(int size)
{
	task_queue_size = size;
}

void ServerMonitor::set_running_workers(int count)
{
	running_workers = count;
}

void ServerMonitor::monite(int timeout)
{
	long long connections = 0;
	long long recv_bytes = 0;
	long long send_bytes = 0;
	long long recv_requests = 0;
	long long send_responses = 0;

	long long last_connections = 0;
	long long last_recv_bytes = 0;
	long long last_send_bytes = 0;
	long long last_recv_requests = 0;
	long long last_send_responses = 0;

	static long long max_recv_data_speed = 0;
	static long long max_send_data_speed = 0;
	static long long max_recv_req_speed = 0;
	static long long max_send_res_speed = 0;

	static std::string max_recv_data_speed_str;
	static std::string max_send_data_speed_str;
	static std::string max_recv_req_speed_str;
	static std::string max_send_res_speed_str;

	while(!m_exit)
	{
		DateTime::sleep(timeout);

		connections = m_connections;
		recv_bytes = m_recv_bytes;
		send_bytes = m_send_bytes;
		recv_requests = m_recv_requests;
		send_responses = m_send_responses;

		std::string recv_byte_speed = speed(recv_bytes-last_recv_bytes, timeout);
		std::string send_byte_speed = speed(send_bytes-last_send_bytes,timeout);
		std::string recv_request_speed = to_string((recv_requests-last_recv_requests)*1000.0/timeout)+"/s";
		std::string send_response_speed = to_string((send_responses-last_send_responses)*1000.0/timeout)+"/s";
		
		if((recv_bytes-last_recv_bytes)*1000.0 / timeout > max_recv_data_speed)
		{
			max_recv_data_speed = (recv_bytes-last_recv_bytes)*1000.0 / timeout;
			max_recv_data_speed_str = recv_byte_speed;
		}
		if((send_bytes-last_send_bytes)*1000.0 / timeout > max_send_data_speed)
		{
			max_send_data_speed = (send_bytes-last_send_bytes)*1000.0 / timeout;
			max_send_data_speed_str = send_byte_speed;
		}
		if((recv_requests-last_recv_requests)*1000.0/timeout > max_recv_req_speed)
		{
			max_recv_req_speed = (recv_requests-last_recv_requests)*1000.0/timeout;
			max_recv_req_speed_str = recv_request_speed;
		}
		if((send_responses-last_send_responses)*1000.0/timeout > max_send_res_speed)
		{
			max_send_res_speed = (send_responses-last_send_responses)*1000.0/timeout;
			max_send_res_speed_str = send_response_speed;
		}
		//printf("\r\e[K");
		clrsr();
		printf("connections:%lld\nrecv request %lld : %s, %s\nsend response %lld : %s, %s\nrecv data %s : %s, %s\nsend data %s : %s, %s\ntask queue size:%lld\nrunning workers:%lld\n",
			connections, recv_requests, recv_request_speed.c_str(), max_recv_req_speed_str.c_str(), 
			send_responses, send_response_speed.c_str(), max_send_res_speed_str.c_str(),
			bytes2str(recv_bytes).c_str(), recv_byte_speed.c_str(), max_recv_data_speed_str.c_str(),
			bytes2str(send_bytes).c_str(), send_byte_speed.c_str(), max_send_data_speed_str.c_str(),
			task_queue_size, running_workers);
		fflush(stdout);

		last_connections = connections;
		last_recv_bytes = recv_bytes;
		last_send_bytes = send_bytes;
		last_recv_requests = recv_requests;
		last_send_responses = send_responses;
	}
}

void ServerMonitor::quit()
{
	m_exit = true;
}
