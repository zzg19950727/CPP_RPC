include_directories(.)
aux_source_directory(. SRC)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(code_gen ${SRC})
