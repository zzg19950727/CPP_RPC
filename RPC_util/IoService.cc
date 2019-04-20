#include "IoService.h"
#include "log_utils.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>

IoService::IoService(int size)
	: m_exit(false)
{
	create_epoll(size);
	std::thread tmp(std::bind(&IoService::poll, this));
	m_thread.swap(tmp);
}

IoService::~IoService()
{
	m_exit.store(true);
	close(m_epoll_fd);
	m_thread.join();
}

void IoService::poll()
{
	log(LOG_USER|LOG_DEBUG, "IoService start poll");
	struct epoll_event event[10];
	while(!m_exit)
	{
		int state = epoll_wait(m_epoll_fd, event, 10, 1000);
		if(state == -1)
		{
			log(LOG_USER|LOG_ERR, "IoService poll error:%d", errno);
			break;
		}
		else if(state == 0)
		{
			continue;
		}
		
		for(int i=0; i<state; ++i)
		{
			if(event[i].events&EPOLLIN)
				response_event(event[i].data.fd,E_READ);
			if(event[i].events&EPOLLOUT)
				response_event(event[i].data.fd,E_WRITE);
		}
	}
	m_epoll_fd = -1;
	log(LOG_USER|LOG_DEBUG, "IoService stop poll");
}

unsigned int epoll_event(IoService::Event e)
{
	if(e == IoService::E_READ)
		return EPOLLIN;
	else if(e == IoService::E_WRITE)
		return EPOLLOUT;
	else
		return EPOLLIN|EPOLLOUT;
}

int IoService::register_io(int fd, Event e, CallbackFunc& f)
{
	log(LOG_USER|LOG_INFO, "IoService register io fd:%d and Event:%d", fd, e);
	struct epoll_event event;
	event.events = epoll_event(e);
	event.data.fd = fd;
	if(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		log(LOG_USER|LOG_ERR, "IoService register io failed:%d",errno);
		return -1;
	}
	
	if(e == E_RW)
	{
		m_callback_map[FdEvent(fd,E_READ)] = f;
		m_callback_map[FdEvent(fd,E_WRITE)] = f;
	}
	else
		m_callback_map[FdEvent(fd,e)] = f;
	return 0;
}

int IoService::unregister_io(int fd, Event e)
{
	log(LOG_USER|LOG_INFO, "IoService unregister io fd:%d and Event:%d", fd, e);
	struct epoll_event event;
	event.events = epoll_event(e);
	event.data.fd = fd;
	if(epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &event) < 0)
	{
		log(LOG_USER|LOG_ERR, "IoService unregister io failed:%d",errno);
		return -1;
	}

	if(e == E_RW)
	{
		m_callback_map.erase(FdEvent(fd,E_READ));
		m_callback_map.erase(FdEvent(fd,E_WRITE));
	}
	else
		m_callback_map.erase(FdEvent(fd,e));
	return 0;
}

void IoService::create_epoll(int size)
{
	m_epoll_fd = epoll_create(size);
	log(LOG_USER|LOG_DEBUG, "IoService create epoll:%d",m_epoll_fd);
}

void IoService::response_event(int fd, Event e)
{
	FdEvent key(fd,e);
	if(m_callback_map.count(key))
		m_callback_map[key](fd, e);
	else if(e != E_WRITE)
		log(LOG_USER|LOG_WARNING , "IoService:unknow fd:%d and Event:%d",fd,e);
}
