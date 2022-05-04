#!/bin/bash

#SBATCH -A ops
#SBATCH -p short
#SBATCH --ntasks-per-node=64
#SBATCH -N 1
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

echo ""
echo "Building."
echo ""

make

echo ""
echo "Running program."
echo ""

./hello

echo ""
echo "Cleaning."
echo ""

make clean