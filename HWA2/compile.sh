#/bin/bash!

module load OpenMPI
mkdir -p build
cd build
cmake .. 
make
