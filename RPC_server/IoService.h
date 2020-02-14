#ifndef IOSERVICE_H
#define IOSERVICE_H
#include "ThreadPool.hpp"
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <map>

class IoService
{
public:
	enum Event{E_READ=0,E_WRITE,E_RW};
	typedef std::function<void(int,Event)> CallbackFunc;
	
	IoService(int size, int threads=20);
	~IoService();
	
	void poll();
	int register_io(int fd, Event e, CallbackFunc& f);
	int unregister_io(int fd, Event e);
	int modify_event(int fd, Event e);

private:
	void create_epoll(int size);
	int add_event(int fd, Event e);
	int remove_event(int fd, Event e);
	void response_event(int fd, Event e);

	typedef std::pair<int,Event> FdEvent;
	
	std::map<FdEvent, CallbackFunc> m_callback_map;
	std::atomic<bool> m_exit;
	RPC::ThreadPool m_pool;
	
	int m_epoll_fd;
	std::recursive_mutex m_mutex;
};

#endif	//IOSERVICE_H
