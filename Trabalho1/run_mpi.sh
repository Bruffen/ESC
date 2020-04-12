#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r662:ppn=24
#PBS -q mei

module load gcc/5.3.0

cd ESC/T1/
cd "NPB3.3.1/NPB3.3-MPI"
mkdir results

for test in ep mg bt
do
    mkdir results/$test
    for class in W A B
    do
        mkdir results/$test/$class
        #for processes in 2 4
        #do
        #done
    done
done

# Create make.def file for current test
cd config/
cp make.def.use make.def

# Create suite.def file
echo -n "" > suite.def
for test in ep mg bt
do
    for class in W A B
    do
        for processes in 2 4
        do
            echo $test $class $processes >> suite.def
        done
    done
done

cd ..
# Build binaries
make clean
make suite

chmod +x ./bin/*

for test in ep mg bt
do
    for class in W A B
    do
        for processes in 2 4
        do
            for i in 0 1 2 3 4 5 6 7 8 9 10
            do
                echo "----------start------------" >> "results/$test/$class/result-$test-$processes.txt"
                mpirun -np $processes ./bin/$test.$class.$processes >> "results/$test/$class/result-$test-$processes.txt"
                echo "-----------end-------------" >> "results/$test/$class/result-$test-$processes.txt"
            done
        done
    done
done