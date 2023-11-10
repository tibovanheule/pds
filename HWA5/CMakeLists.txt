cmake_minimum_required(VERSION 3.0.0)
project(hwa5 VERSION 0.1.0)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

find_package(MPI REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(hwa5 src/main.cpp src/changroberts.cpp src/franklin.cpp)

target_link_libraries(hwa5 ${MPI_LIBRARIES})
