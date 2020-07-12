#!/bin/sh

#PBS -N "ex2tut3"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r431:ppn=24

module load gcc/5.3.0

cd ESC/T4/ex2
make clean
make clean2
make all
dir="tut3"
mkdir $dir

# Make measurements
( perf stat -r 10 -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses ./naive  ) >> $dir/measurements_ijk.txt 2>&1
( perf stat -r 10 -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses ./naive2 ) >> $dir/measurements_ikj.txt 2>&1


perf record -o tut3naive.data -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses -c 100000 ./naive
perf record -o tut3inter.data -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses -c 100000 ./naive2

perf report -i tut3naive.data -n --no-source --stdio --percent-limit 0.1 > $dir/report_naive.txt
perf report -i tut3inter.data -n --no-source --stdio --percent-limit 0.1 > $dir/report_inter.txt

perf annotate -i tut3naive.data --stdio --dsos=naive --symbol=multiply_matrices --no-source > $dir/annotate_naive.txt
perf annotate -i tut3inter.data --stdio --dsos=naive2 --symbol=multiply_matrices --no-source > $dir/annotate_inter.txt

perf evlist -i tut3naive.data -F > $dir/evlist_naive.txt
perf evlist -i tut3inter.data -F > $dir/evlist_inter.txt

perf report -i tut3naive.data --header-only > $dir/report2_naive.txt
perf report -i tut3inter.data --header-only > $dir/report2_inter.txt

# FlameGraph

perf record -ag -o fgnaive.data -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses -c 100000 ./naive
perf record -ag -o fginter.data -e cpu-cycles,instructions,cache-references,cache-misses,branches,branch-misses,branch-loads,branch-load-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-store-misses,L1-icache-loads,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-load-misses,dTLB-store-misses,iTLB-load-misses -c 100000 ./naive2

perf script -i fgnaive.data | ../FlameGraph/stackcollapse-perf.pl | ../FlameGraph/flamegraph.pl > $dir/naive.svg
perf script -i fginter.data | ../FlameGraph/stackcollapse-perf.pl | ../FlameGraph/flamegraph.pl > $dir/inter.svg

mv tut3naive.data $dir/naive.data
mv tut3inter.data $dir/inter.data

mv fgnaive.data $dir/fg_naive.data
mv fginter.data $dir/fg_inter.data