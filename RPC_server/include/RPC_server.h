#ifndef RPC_SERVER_H
#define RPC_SERVER_H
#include <string>

void run();
typedef size_t (*RPC_func_type)(unsigned char*,size_t);
class RegisHelper
{
public:
	RegisHelper(const std::string& func_name, RPC_func_type func);
};

#endif	//RPC_SERVER_H

