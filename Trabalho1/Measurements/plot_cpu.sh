#!/bin/bash

for c in EP MG BT
do
    #for t in 16 32
    #do
        #./graph_cpu.py omp/mp_${c}_omp${t}.txt
        ./graph_cpu.py gccO0/mp_${c}_gccO0.txt
        ./graph_cpu.py gccO3/mp_${c}_gccO3.txt
    #done
done