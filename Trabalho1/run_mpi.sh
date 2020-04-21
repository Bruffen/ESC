#!/bin/bash

#PBS -N "npb"
#PBS -l walltime=3:00:00
#PBS -l nodes=4:r641:ppn=16

module load gcc/5.3.0
module load gnu/openmpi_eth/2.0.0

cd ESC/T1/
cd "NPB3.3.1/NPB3.3-MPI"
mkdir results641

for test in ep mg bt
do
    mkdir results641/$test
    for class in W A B
    do
        mkdir results641/$test/$class
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
        for processes in 16 32 48 64
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
        for processes in 16 32 48 64 # 1 2 4
        do
            for i in 0 1 2 3 4 5 6 7 8 9 10
            do
                if [[ $test == "bt" && $processes == "2" ]]; then
                    echo ""
                else
                    echo "----------start------------" >> "results641/$test/$class/result-$test-$processes.txt"
                    mpirun -np $processes -mca btl self,sm,tcp -oversubscribe --map-by core ./bin/$test.$class.$processes >> "results641/$test/$class/result-$test-$processes.txt"
                    echo "-----------end-------------" >> "results641/$test/$class/result-$test-$processes.txt"
                fi
            done
        done
    done
done
