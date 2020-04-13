#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=3:00:00
#PBS -l nodes=1:r641:ppn=16

module load gcc/5.3.0

cd ESC/T1/
cd "NPB3.3.1-MZ/NPB3.3-MZ-OMP"
mkdir results

for test in bt-mz
do
    mkdir results/$test
    for class in W A B
    do
        for threads in 1 4 8 12 16
        do
            mkdir results/$test/$class
        done
    done
done

# Create make.def file for current test
cd config/
cp make.def.use make.def

# Create suite.def file
echo -n "" > suite.def
for test in bt-mz
do
    for class in W A B
    do
        #for threads in 4 8 12
        #do
            echo $test $class >> suite.def #$threads# >> suite.def
        #done
    done
done

cd ..
# Build binaries
make clean
make suite

chmod +x ./bin/*

for test in bt-mz
do
    for class in W A B
    do
        for threads in 1 4 8 12 16
        do
            export OMP_NUM_THREADS=$threads
            for i in 0 1 2 3 4 5 6 7 8 9 10
            do
                echo "----------start------------" >> "results/$test/$class/result-$test-$threads.txt"
                ./bin/$test.$class.x >> "results/$test/$class/result-$test-$threads.txt"
                echo "-----------end-------------" >> "results/$test/$class/result-$test-$threads.txt"
            done
        done
    done
done