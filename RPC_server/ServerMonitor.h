#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

class ServerMonitor
{
public:
	ServerMonitor();
	~ServerMonitor();
	void add_connections(int n=1);
	void remove_connections(int n=1);
	void recv_bytes(int n);
	void send_bytes(int n);
	void recv_requests(int n=1);
	void send_responses(int n=1);
	void monite(int timeout);
	void quit();

private:
	bool m_exit;
	long long m_connections;
	long long m_recv_bytes;
	long long m_send_bytes;
	long long m_recv_requests;
	long long m_send_responses;
};

#endif	//SERVER_MONITOR_H
