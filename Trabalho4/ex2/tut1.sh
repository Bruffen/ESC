#!/bin/sh

#PBS -N "ex2tut1"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r431:ppn=24

module load gcc/5.3.0

cd ESC/T4/ex2
make clean
make naive
dir="tut1"
mkdir $dir

# Elapsed time and event counting
( perf stat -e cpu-clock,faults ./naive ) >> $dir/step1_baseline.txt 2>&1


# How do I find the hot spots?
# Step 1: Collect profile data

perf record -o naive1.data -e cpu-clock,faults ./naive

# Step 2: Display profiles (stdio)
perf report -i naive1.data --stdio --sort comm,dso > $dir/naive1report1.txt
perf report -i naive1.data --stdio --dsos=naive,libc-2.12.so > $dir/naive1report2.txt
perf annotate -i naive1.data --stdio --dsos=naive --symbol=multiply_matrices > $dir/naive1annotate1.txt
perf annotate -i naive1.data --stdio --dsos=naive --symbol=multiply_matrices --no-source > $dir/naive1annotate2.txt
mv naive1.data $dir/naive1.data
