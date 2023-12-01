#!/bin/bash

module load intel/2023a
module load CMake/3.26.3-GCCcore-12.3.0
rm -r build
mkdir -p build
cd build
cmake ..
make

cp hwa5 ../.
