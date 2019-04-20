CC=g++
CFLAGS=-std=c++11 -g
PROTOBUF_PATH=$(PWD)/../protobuf
PROTOC=$(PROTOBUF_PATH)/bin/protoc
INCLUDE_DIR=$(PROTOBUF_PATH)/include
LIB_DIR=$(PROTOBUF_PATH)/lib
RPC_CLIENT_INCLUDE_DIR=$(PWD)/../RPC_client/include
RPC_CLIENT_LIB_DIR=$(PWD)/../RPC_client/lib
RPC_SERVER_INCLUDE_DIR=$(PWD)/../RPC_server/include
RPC_SERVER_LIB_DIR=$(PWD)/../RPC_server/lib

all:src RPC_client RPC_server

src:
	sh $(PWD)/build.sh

RPC_client:test.rpc.pb.o test.rpc.client.o test.rpc.client_test.o
	$(CC) $(CFLAGS) $^ -L$(RPC_CLIENT_LIB_DIR) -lRPC_client -L$(LIB_DIR) -lprotobuf -lpthread -o RPC_client

RPC_server:add.o test.rpc.pb.o test.rpc.server.o test.rpc.server_test.o
	$(CC) $(CFLAGS) $^ -L$(RPC_SERVER_LIB_DIR) -lRPC_server -L$(LIB_DIR) -lprotobuf -lpthread -o RPC_server

test.rpc.client_test.o:test.rpc.client_test.cc
	$(CC) $(CFLAGS) -I$(RPC_CLIENT_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^

test.rpc.server_test.o:test.rpc.server_test.cc
	$(CC) $(CFLAGS) -I$(RPC_SERVER_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^

add.o:add.cc
	$(CC) $(CFLAGS) -c $^

test.rpc.pb.o:test.rpc.pb.cc
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $^

test.rpc.client.o:test.rpc.client.cc
	$(CC) $(CFLAGS) -I$(RPC_CLIENT_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^

test.rpc.server.o:test.rpc.server.cc
	$(CC) $(CFLAGS) -I$(RPC_SERVER_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $^

test.rpc.pb.cc :
	$(PROTOC) test.rpc.proto --cpp_out=./

clean :
	rm -f *.o *.proto test.rpc.client.cc test.rpc.server.cc test.rpc.pb.* RPC_server RPC_client
