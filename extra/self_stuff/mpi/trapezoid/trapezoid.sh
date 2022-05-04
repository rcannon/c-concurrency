#!/bin/bash

#SBATCH -A br22_cann484
#SBATCH -o out%J.out
#SBATCH -e err%J.out
#SBATCH -N 1

source /etc/profile.d/modules.sh
module purge
#module load cmake/3.19.6
module load gcc/9.1.0
module load openmpi/4.1.2rc1

mpicc -g -Wall -o hello mpi_hello_world.c

mpiexec -n 8 ./hello
