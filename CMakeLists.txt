cmake_minimum_required(VERSION 3.0)
project(test1_usb)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

aux_source_directory(. DIR_SRCS)
add_executable(test1 ${DIR_SRCS})

target_link_libraries(test1 usb-1.0)