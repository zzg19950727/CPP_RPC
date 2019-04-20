#include "RPC_client_service.h"
#include "ResponseHandle.h"
#include <cstring>

ResponseHandle::ResponseHandle(RPC::Condition& condition,
	TimerManager& manager,int tag)
	:m_condition(condition),
	m_timer_manager(manager),
	m_timer(manager, Timer::CIRCLE),
	m_state(S_WAITING),
	m_tag(tag)
{

}

ResponseHandle::~ResponseHandle()
{
	buffer_free(m_buffer);
}

size_t ResponseHandle::read(char* buf,size_t max_len, int timeout)
{
	Timer::CallbackFunc func = std::bind(&ResponseHandle::time_out, this);
	m_timer.set_callback_func(func);
	m_timer.start(timeout);

	m_condition.wait(m_tag);
	
	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_state == S_TIME_OUT)
		return 0;
	if(m_buffer.data && max_len > m_buffer.length)
	{
		memcpy(buf, m_buffer.data, m_buffer.length);
	}
	return m_buffer.length;
}

size_t ResponseHandle::write(char* buf, size_t len)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if(m_state == S_TIME_OUT)
		return 0;
	buffer_realloc(m_buffer, len);
	if(m_buffer.capacity >= len)
	{
		memcpy(m_buffer.data,buf,len);
		m_buffer.length = len;
		
	}
	m_state = S_DONE;
	m_timer.start(100);
	m_condition.notify_all(m_tag);
	return len;
}

void ResponseHandle::time_out()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	
	if(m_state == S_WAITING)
		m_state = S_TIME_OUT;
	m_condition.notify_all(m_tag);
}
