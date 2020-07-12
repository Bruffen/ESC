#!/bin/sh

#PBS -N "ex2tut2"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r431:ppn=24

module load gcc/5.3.0

cd ESC/T4/ex2
make clean
make clean2
make all
dir="tut2"
mkdir $dir

# Make measurements
( perf stat -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses ./naive  ) >> $dir/measurements_ijk.txt 2>&1
( perf stat -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses ./naive2 ) >> $dir/measurements_ikj.txt 2>&1
