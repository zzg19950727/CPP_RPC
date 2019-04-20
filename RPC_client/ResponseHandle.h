#ifndef RESPONSE_HANDLE_H
#define RESPONSE_HANDLE_H
#include "ThreadPool.hpp"
#include "TimerManager.h"
#include "buffer.h"
#include <mutex>

enum State
{
	S_WAITING=0,
	S_DONE,
	S_TIME_OUT,
	S_LOCAL_CACHE_FULL,
	S_SERVER_ERROR,
	S_NO_CONNECTION
};

class ResponseHandle
{
public:
	ResponseHandle(RPC::Condition& condition,
	TimerManager& manager,int tag);
	~ResponseHandle();
	size_t read(char* buf,size_t max_len, int timeout);
	size_t write(char* buf, size_t len);
	
private:
	void time_out();

	TimerManager& m_timer_manager;
	RPC::Condition& m_condition;
	Buffer m_buffer;
	Timer m_timer;
	std::mutex m_mutex;
	State m_state;
	int m_tag;
};

#endif	//RESPONSE_HANDLE_H
