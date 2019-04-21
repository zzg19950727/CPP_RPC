CC=g++
CFLAGS=-g -shared -fPIC -std=c++11
PROTOBUF_PATH=$(PWD)/../protobuf
PROTOC=$(PROTOBUF_PATH)/bin/protoc
PROTO_INCLUDE_DIR=$(PROTOBUF_PATH)/include
PROTO_LIB_DIR=$(PROTOBUF_PATH)/lib
UTIL_DIR=$(PWD)/../RPC_util
INCLUDE_DIR=$(PWD)/include
LIB_DIR=$(PWD)/lib

SOURCE=RequestHandle.cc \
RPC_server.cc \
server_service_config.cc \
RPC_template.pb.cc \
ServerMonitor.cc \
IoService.cc \
$(UTIL_DIR)/TimerManager.cc \
$(UTIL_DIR)/buffer.cc \
$(UTIL_DIR)/log_utils.cc

RPC_client:$(SOURCE)
	$(CC) $(CFLAGS) -I$(UTIL_DIR) -I$(INCLUDE_DIR) -I$(PROTO_INCLUDE_DIR) -L$(PROTO_LIB_DIR) -lprotobuf -lpthread $^ -o $(LIB_DIR)/libRPC_server.so

RPC_template.pb.cc:RPC_template.proto
	$(PROTOC) $^ --cpp_out=./

clean:
	rm -f $(LIB_DIR)/libRPC_server.so RPC_template.pb.*
