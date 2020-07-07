#!/bin/sh

#PBS -N "ex1a"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r641:ppn=32
#PBS -q mei

module load gcc/5.3.0

cd ESC/T4/ex1
make clean
make
dir="ex1aout"
mkdir $dir

for i in 1 2 3 4
do
    ( perf stat -e cpu-clock,cache-references,cache-misses,cycles,instructions,branches,branch-misses,faults,migrations ./sort $i 1 1000000000 ) >> $dir/$i.txt 2>&1
done