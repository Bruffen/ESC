#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

module load intel/2013.1.117
source /share/apps/intel/compilers_and_libraries_2017/linux/bin/compilervars.sh intel64

cd ESC/T1/
cd "NPB3.3.1/NPB3.3-SER"
mkdir results

    mkdir results/iccO3
    for test in ep mg bt
    do
        mkdir results/iccO3/$test
        for class in W A B
        do
            mkdir results/iccO3/$test/$class
        done
    done

    # Create make.def file for current test
    cd config/
    cp make.def.iccO3 make.def

    # Create suite.def file
    echo -n "" > suite.def
    for test in ep mg bt
    do
        for class in W A B
        do
            echo $test $class >> suite.def
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
            for i in 0 1 2 3 4 5 6 7 8 9 10
            do
                echo "----------start------------" >> "results/iccO3/$test/$class/result-iccO3-$test.txt"
                ./bin/$test.$class.x >> "results/iccO3/$test/$class/result-iccO3-$test.txt"
                echo "-----------end-------------" >> "results/iccO3/$test/$class/result-iccO3-$test.txt"
            done
        done
    done