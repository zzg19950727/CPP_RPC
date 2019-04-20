#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H
#include <unistd.h>
#include <string>
#define BLOCK	1024
size_t send_request(const std::string& method, int query_id, unsigned char* buf, size_t len);

#endif	//RPC_CLIENT_H
