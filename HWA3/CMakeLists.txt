cmake_minimum_required(VERSION 3.22)
project(HWA3)

set(CMAKE_CXX_STANDARD 11)

add_executable(HWA3 main.cpp results/results.cpp algorithms/raymond/raymond.cpp algorithms/ring/ring.cpp algorithms/central/central.cpp)
