#!/bin/sh

#PBS -N "ex1b"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r641:ppn=32
#PBS -q mei

module load gcc/5.3.0

cd ESC/T4/ex1
make clean
make
dir="ex1dout"
mkdir $dir

for i in 1 2 3 4
do
    perf record -g -o ${i}d.data ./sort 1 1 100000000
    perf annotate -i ${i}d.data --stdio > $dir/$i.txt
    mv ${i}d.data $dir/${i}d.data
done