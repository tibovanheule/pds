# Project
project(allreduce)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Version Number
set(allreduce_VERSION_MAJOR 1)
set(allreduce_VERSION_MINOR 0)

# CMAKE version
cmake_minimum_required(VERSION 2.6)

# Find the MPI library
find_package(MPI REQUIRED)

set (CMAKE_CXX_FLAGS "-O3")
add_executable(allreduce allreduce.cpp)

# link with the MPI library
target_link_libraries(allreduce ${MPI_LIBRARIES})
