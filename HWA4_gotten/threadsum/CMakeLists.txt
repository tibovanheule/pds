# Project
project(ThreadSum)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Version Number
set(matmat_VERSION_MAJOR 1)
set(matmat_VERSION_MINOR 0)

# CMAKE version
cmake_minimum_required(VERSION 2.6)

find_package(Threads REQUIRED)

# set the flags to -O0 (DON'T OPTIMIZE !!)
set (CMAKE_CXX_FLAGS "-O0 -std=c++11")

add_executable(threadsum threadsum.cpp)

target_link_libraries(threadsum ${CMAKE_THREAD_LIBS_INIT})
