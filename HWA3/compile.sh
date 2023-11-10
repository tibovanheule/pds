#/bin/bash!

module load intel/2021b
module load CMake/3.22.1-GCCcore-11.2.0
mkdir -p build
cd build
cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
make
