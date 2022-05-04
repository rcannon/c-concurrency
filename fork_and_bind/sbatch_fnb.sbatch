#!/bin/bash

#SBATCH -A ops
#SBATCH -p short
##SBATCH --ntasks-per-node=64
##SBATCH -N 1
#SBATCH -t 10
#SBATCH -o out%J.out
#SBATCH -e err%J.out
#SBATCH -J fnb

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
ldd bin/fork_and_bind
echo 
echo program output
echo 

echo 
echo "Remove old thread files."
echo 

rm ./thread_files/*

echo 
echo "Running program."
echo 

touch start.time
bin/fork_and_bind 64
touch end.time

sleep 5

echo 
echo "Cleaning."
echo 

make clean

echo 
echo "Times."
echo 

../seconds_between.pl start.time end.time

rm start.time
rm end.time

