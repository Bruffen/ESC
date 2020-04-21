#!/bin/bash

for c in EP MG BT
do
    for t in 16 32
    do
        ./graph_cpu.py omp/mp_${c}_omp${t}.txt
    done
done