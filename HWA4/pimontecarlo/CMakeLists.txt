# Project
project(PiMonteCarlo)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Version Number
set(matmat_VERSION_MAJOR 1)
set(matmat_VERSION_MINOR 0)

# CMAKE version
cmake_minimum_required(VERSION 2.6)

find_package(Threads REQUIRED)

# set the flags to -O3
set (CMAKE_CXX_FLAGS "-O3 -std=c++11")

add_executable(pimontecarlo pimontecarlo.cpp)

target_link_libraries(pimontecarlo ${CMAKE_THREAD_LIBS_INIT})
