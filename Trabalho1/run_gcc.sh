#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=3:00:00
#PBS -l nodes=1:r662:ppn=24
#PBS -q mei

module load gcc/5.3.0
module load intel/2013.1.117
source /share/apps/intel/compilers_and_libraries_2017/linux/bin/compilervars.sh intel64

cd ESC/T1/
cd "NPB3.3.1/NPB3.3-SER"
mkdir results

for test_option in gccO0 gccO1 gccO2 gccO3 iccO3
do
    mkdir results/$test_option
    for test in ep mg bt
    do
        mkdir results/$test_option/$test
        for class in S #W A B
        do
            mkdir results/$test_option/$test/$class
        done
    done

    # Create make.def file for current test
    cd config/
    cp make.def.$test_option make.def

    # Create suite.def file
    echo -n "" > suite.def
    for test in ep mg bt
    do
        for class in S #W A B
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
        for class in S #W A B
        do
            for i in 0 1 2 3 4 5 6 7 8 9 10
            do
                echo "----------start------------" >> "results/$test_option/$test/$class/result-$test_option-$test.txt"
                ./bin/$test.$class.x >> "results/$test_option/$test/$class/result-$test_option-$test.txt"
                echo "-----------end-------------" >> "results/$test_option/$test/$class/result-$test_option-$test.txt"
            done
        done
    done
done