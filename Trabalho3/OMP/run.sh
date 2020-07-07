#!/bin/sh

make
./distance_transform "../img${1}.pgm" ${2} 8 > "./results${1}_${2}threads_8dynamic.txt"
