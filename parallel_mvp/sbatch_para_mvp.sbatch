#!/bin/bash

#SBATCH -A ops
#SBATCH -p fat
##SBATCH --ntasks-per-node=64
#SBATCH -N 1
#SBATCH -t 10
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

rm -rf thread_files

echo 
echo "Running program."
echo 

touch /tmp/start.time
bin/mvp 15 2000 64
touch /tmp/end.time

sleep 5

echo 
echo "Cleaning."
echo 

make clean

echo 
echo "Times."
echo 

mkdir thread_files
mv /scratch/out_* thread_files/

../seconds_between.pl /tmp/start.time /tmp/end.time


