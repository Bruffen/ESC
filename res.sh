#!/bin/sh

for test_option in gccO0 gccO1 gccO2 gccO3 iccO3
do
    for test in ep mg bt
    do
        for class in S #W A B
        do
            ./results.py Trabalho1/results/ser/results641/$test_option/$test/$class >> "res/result-$test_option-$test-$class.txt"
            
        done
    done
done
