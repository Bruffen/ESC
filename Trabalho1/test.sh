#!/bin/bash

for cores in 16
do
for test in bt-mz
do
    for class in W A B
    do
        for processes in 1 2 3 4
        do
                echo $processes
                OMP_NUM_THREADS=$((processes*cores))
                echo $OMP_NUM_THREADS
        done
    done
done
done