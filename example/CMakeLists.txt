set(PROTO_SRCS
    test.rpc.client.cc
    test.rpc.server.cc
    test.rpc.pb.cc)

add_custom_command(
	OUTPUT ${PROTO_SRCS}
        PRE_BUILD
	COMMAND ${PROJECT_SOURCE_DIR}/CodeGen/code_gen
		${CMAKE_CURRENT_SOURCE_DIR}/test.rpc ./
        COMMAND ${PROJECT_SOURCE_DIR}/protobuf/bin/protoc
                -I${CMAKE_CURRENT_BINARY_DIR}
                --cpp_out=./
		${CMAKE_CURRENT_BINARY_DIR}/test.rpc.proto
        DEPENDS test.rpc
)

include_directories(.
    ${PROJECT_SOURCE_DIR}/CPP_util
    ${PROJECT_SOURCE_DIR}/RPC_server
    ${PROJECT_SOURCE_DIR}/RPC_client
    ${CMAKE_CURRENT_BINARY_DIR}
    )

set(CLIENT_SRC
	${CMAKE_CURRENT_BINARY_DIR}/test.rpc.pb.cc
	${CMAKE_CURRENT_BINARY_DIR}/test.rpc.client.cc
	test.rpc.client_test.cc
	)

set(SERVER_SRC
	add.cc
	${CMAKE_CURRENT_BINARY_DIR}/test.rpc.pb.cc
	${CMAKE_CURRENT_BINARY_DIR}/test.rpc.server.cc
	test.rpc.server_test.cc
	)

set(TEST_SRC
	test.cc
	)

add_executable(RPC_client ${CLIENT_SRC})
add_executable(RPC_server ${SERVER_SRC})
add_executable(RPC_test ${TEST_SRC})
target_link_libraries(RPC_client pthread protobuf util rpc_client_lib)
target_link_libraries(RPC_server util rpc_server_lib protobuf pthread)
target_link_libraries(RPC_test pthread)
