#ifndef IOSERVICE_H
#define IOSERVICE_H
#include <functional>
#include <atomic>
#include <thread>
#include <map>

class IoService
{
public:
	enum Event{E_READ=0,E_WRITE,E_RW};
	typedef std::function<void(int,Event)> CallbackFunc;
	
	IoService(int size);
	~IoService();
	
	void poll();
	int register_io(int fd, Event e, CallbackFunc& f);
	int unregister_io(int fd, Event e);
private:
	void create_epoll(int size);
	void response_event(int fd, Event e);

	typedef std::pair<int,Event> FdEvent;
	
	std::atomic<bool> m_exit;
	int m_epoll_fd;
	std::thread m_thread;
	std::map<FdEvent, CallbackFunc> m_callback_map;
};

#endif	//IOSERVICE_H
