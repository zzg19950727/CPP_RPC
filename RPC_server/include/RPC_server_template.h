#ifndef RPC_SERVER_TEMPLATE_H
#define RPC_SERVER_TEMPLATE_H
#include <google/protobuf/io/coded_stream.h>
#include "RPC_server.h"
#include <unistd.h>

#define RPC_SERVER0(result, func)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func());\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#define RPC_SERVER1(result, func, arg1)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func(\
				request.arg1()));\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#define RPC_SERVER2(result, func, arg1, arg2)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func(\
				request.arg1(), \
				request.arg2()));\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#define RPC_SERVER3(result, func, arg1, arg2, arg3)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func(\
				request.arg1(), \
				request.arg2(),\
				request.arg3()));\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#define RPC_SERVER4(result, func, arg1, arg2, arg3, arg4)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func(\
				request.arg1(), \
				request.arg2(),\
				request.arg3(),\
				request.arg4()));\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#define RPC_SERVER5(result, func, arg1, arg2, arg3, arg4, arg5)\
size_t RPC_##func(unsigned char* buf, size_t len)\
{\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	RPC_##func##_request request;\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	request.MergePartialFromCodedStream(&input);\
	RPC_##func##_response response;\
	if (request.method() != #func)\
		return 0;\
	response.set_method(request.method());\
	response.set_query_id(request.query_id());\
	response.set_##result(func(\
				request.arg1(), \
				request.arg2(),\
				request.arg3(),\
				request.arg4(),\
				request.arg5()));\
	len = response.ByteSize();\
	response.SerializeWithCachedSizesToArray(buf);\
	return len;\
}\
RegisHelper g_help_##func##_regis(#func, RPC_##func);

#endif	//RPC_SERVER_TEMPLATE_H
