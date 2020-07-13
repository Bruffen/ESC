#!/bin/sh

dir="test"
mkdir $dir

mpicc -o distance_transform distance_transform.c
for i in 0
do
    echo "\n\nREPETITION $i:\n" >> "./${dir}/results${1}_${2}processes.txt"
    mpirun -np ${2} distance_transform "../img${1}.pgm" >> "./${dir}/results${1}_${2}processes.txt"
done