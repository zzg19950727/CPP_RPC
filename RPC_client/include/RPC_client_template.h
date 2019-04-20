#ifndef RPC_CLIENT_TEMPLATE_H
#define RPC_CLIENT_TEMPLATE_H
#include <google/protobuf/io/coded_stream.h>
#include "RPC_client.h"

#define RPC_CLIENT0(result, func)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#define RPC_CLIENT1(result, func, arg1)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_##arg1(arg1);\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#define RPC_CLIENT2(result, func, arg1, arg2)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_##arg1(arg1);\
	request.set_##arg2(arg2);\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#define RPC_CLIENT3(result, func, arg1, arg2, arg3)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_##arg1(arg1);\
	request.set_##arg2(arg2);\
	request.set_##arg3(arg3);\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#define RPC_CLIENT4(result, func, arg1, arg2, arg3, arg4)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_##arg1(arg1);\
	request.set_##arg2(arg2);\
	request.set_##arg3(arg3);\
	request.set_##arg4(arg4);\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#define RPC_CLIENT5(result, func, arg1, arg2, arg3, arg4, arg5)\
	using RPC::RPC_##func##_request;\
	using RPC::RPC_##func##_response;\
	static int query_id = 1;\
	RPC_##func##_request request;\
	request.set_##arg1(arg1);\
	request.set_##arg2(arg2);\
	request.set_##arg3(arg3);\
	request.set_##arg4(arg4);\
	request.set_##arg5(arg5);\
	request.set_query_id(query_id++);\
	request.set_method(#func);\
	RPC_##func##_response response;\
	size_t len = request.ByteSize();\
	unsigned char buf[BLOCK];\
	request.SerializeWithCachedSizesToArray(buf);\
	len = send_request(#func, request.query_id(), buf,len);\
	::google::protobuf::io::CodedInputStream input(buf,len);\
	response.MergePartialFromCodedStream(&input);\
	return response.result();

#endif	//RPC_CLIENT_TEMPLATE_H
