#!/bin/bash

#SBATCH -A ops
#SBATCH -p short
##SBATCH --ntasks-per-node=64
#SBATCH -N 1
#SBATCH -t 60
#SBATCH -o out%J.out
#SBATCH -e err%J.out
#SBATCH -J mvp

source /etc/profile.d/modules.sh
source ../env.sh

ulimit -s

echo module list output
echo
module list > ml.lis 2>&1
cat ml.lis
/bin/rm -f ml.lis

echo
echo printenv output
echo
printenv

echo
echo limit output
echo
ulimit -a

echo 
echo "Building."
echo 

make clean
make

echo
echo ldd output
echo 
ldd bin/mvp
echo 
echo program output
echo 

echo 
echo "Remove old thread files."
echo 

rm thread_files/*

echo
echo "Max shared memory size (bytes)"
echo 

cat /proc/sys/kernel/shmmax

echo 
echo "Running program."
echo 

touch /scratch/start.time
bin/mvp 63 128 1243 # maximum size
touch /scratch/end.time

sleep 5

echo 
echo "Cleaning."
echo 

make clean

echo 
echo "Times."
echo 

mv /scratch/out_* thread_files/
../seconds_between.pl /scratch/start.time /scratch/end.time

echo
echo "ipcs -a: make sure shm segments are destroyed"
echo 

ipcs -a


