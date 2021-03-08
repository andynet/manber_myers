cmake_minimum_required(VERSION 3.17)
project(bwt)

set(CMAKE_C_STANDARD 11)

add_library(hash_table src/hash_table.c src/hash_table.h)

add_executable(bwt src/bwt.c)

target_link_libraries(bwt hash_table)
target_link_libraries(bwt m)    # math
