#ifndef BUFFER_H
#define BUFFER_H
#include <mutex>

struct Header
{
	int magic;
	int length;
	Header()
	:length(0)
	{}
};

struct Buffer
{
	int capacity;
	int length;
	char* data;
	Buffer()
	:capacity(0),
	length(0),
	data(0)
	{}
};

void buffer_copy(Buffer& buf, const char* ptr, unsigned int len);
void buffer_calloc(Buffer& buf, unsigned int len);
void buffer_realloc(Buffer& buf, unsigned int len);
void buffer_free(Buffer& buf);

class BufferCacheBase
{
public:
	BufferCacheBase(int size);
	~BufferCacheBase();
	int size()const;
	int capacity()const;
	bool is_empty()const;
	bool is_full()const;
	int have_a_look(Buffer& buf);
	int read(Buffer& buf);
	int write(const Buffer& buf);

protected:
	Buffer m_buffer;
	int m_head,m_tail;
	mutable std::recursive_mutex m_mutex;
};

class BufferCache : public BufferCacheBase
{
public:
	BufferCache(int size);
	~BufferCache();
	bool write_package(const char* buf, unsigned int len);
	bool read_package(Header& head, Buffer& buf);
};

#endif	//BUFFER_H
