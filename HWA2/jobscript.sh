#!/bin/sh
#
#
#PBS -N allreduce
#PBS -o output.file
#PBS -e error.file
#PBS -q short
#PBS -l nodes=8:ppn=8
#PBS -l walltime=00:10:00
#PBS -m n
#

module load OpenMPI

cd $VSC_HOME/your-directory
mpirun ./allreduce
