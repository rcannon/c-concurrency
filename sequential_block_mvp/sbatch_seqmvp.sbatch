#!/bin/bash

#SBATCH -A ops
#SBATCH -p dl_shared
##SBATCH --ntasks-per-node=64
#SBATCH -N 1
#SBATCH -t 120
#SBATCH -o out%J.out
#SBATCH -e err%J.out
#SBATCH -J seq_mvp

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

make

echo
echo ldd output
echo 
ldd bin/mvp
echo 
echo program output
echo 

echo 
echo "Running program."
echo 

#bin/mvp 200 200 1 1

touch /scratch/start1.time
bin/mvp 230 203 1 0
touch /scratch/end1.time

#touch /scratch/start2.time
#bin/mvp 450 450 20 0
#touch /scratch/end2.time

#sleep 5

echo 
echo "Cleaning."
echo 

make clean

echo 
echo "Times."
echo 

../seconds_between.pl /scratch/start1.time /scratch/end1.time
#../seconds_between.pl /scratch/start2.time /scratch/end2.time


