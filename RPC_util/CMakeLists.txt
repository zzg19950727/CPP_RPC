include_directories(.)
aux_source_directory(. SRC)
add_library(util ${SRC})
