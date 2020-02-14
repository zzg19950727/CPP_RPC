cmake_minimum_required(VERSION 2.8.0)
project(CPP_RPC CXX)

set (CMAKE_CXX_FLAGS  "-g -std=c++11")

include_directories(
	${PROJECT_SOURCE_DIR}/example
	${PROJECT_SOURCE_DIR}/protobuf/include
	${PROJECT_SOURCE_DIR}/RPC_client
	${PROJECT_SOURCE_DIR}/RPC_server
	${PROJECT_SOURCE_DIR}/RPC_util
)

add_subdirectory(CodeGen)
add_subdirectory(example)
add_subdirectory(RPC_client)
add_subdirectory(RPC_server)
add_subdirectory(RPC_util)

set(LIB_INC_DIR ${PROJECT_SOURCE_DIR}/protobuf/include)
set(LIB_LINK_DIR ${PROJECT_SOURCE_DIR}/protobuf/lib)

include_directories(${LIB_INC_DIR})
link_directories(${LIB_LINK_DIR})
link_libraries(protobuf)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
