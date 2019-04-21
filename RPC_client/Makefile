CC=g++
CFLAGS=-g -std=c++11 -shared -fPIC
PROTOBUF_PATH=$(PWD)/../protobuf
PROTOC=$(PROTOBUF_PATH)/bin/protoc
PROTO_INCLUDE_DIR=$(PROTOBUF_PATH)/include
PROTO_LIB_DIR=$(PROTOBUF_PATH)/lib
UTIL_DIR=$(PWD)/../RPC_util
INCLUDE_DIR=$(PWD)/include
LIB_DIR=$(PWD)/lib

SOURCE=ResponseHandle.cc \
RPC_client.cc \
client_service_config.cc \
RPC_template.pb.cc \
IoService.cc \
$(UTIL_DIR)/buffer.cc \
$(UTIL_DIR)/TimerManager.cc \
$(UTIL_DIR)/log_utils.cc

RPC_client:$(SOURCE)
	$(CC) $(CFLAGS) -I$(UTIL_DIR) -I$(INCLUDE_DIR) -I$(PROTO_INCLUDE_DIR) -L$(PROTO_LIB_DIR) -lprotobuf -lpthread $^ -o $(LIB_DIR)/libRPC_client.so

RPC_template.pb.cc:RPC_template.proto
	$(PROTOC) $^ --cpp_out=./

clean:
	rm -f $(LIB_DIR)/libRPC_client.so RPC_template.pb.*
