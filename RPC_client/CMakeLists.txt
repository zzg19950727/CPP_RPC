set(PROTO_SRCS
    RPC_template.pb.h
    RPC_template.pb.cc)

add_custom_command(
	OUTPUT ${PROTO_SRCS}
        PRE_BUILD
        COMMAND ${PROJECT_SOURCE_DIR}/protobuf/bin/protoc
                -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=./
		${CMAKE_CURRENT_SOURCE_DIR}/RPC_template.proto
        DEPENDS RPC_template.proto
	COMMENT "Now Generating protobuf source file for RPC client"
)

aux_source_directory(. OTHER_SRCS)
include_directories(.
    ${PROJECT_SOURCE_DIR}/CPP_util
    ${CMAKE_CURRENT_BINARY_DIR}
	)

add_library(rpc_client_lib ${PROTO_SRCS} ${OTHER_SRCS})
target_link_libraries(rpc_client_lib util protobuf)
