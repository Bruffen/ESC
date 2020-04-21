#!/bin/bash

step=6
curr=0

for v in {0..31}
do
    s=$(( $v*$step ))
    echo "\addplot [color={rgb,255:red,255;green,$s;blue,$s}, mark=*] table {Data/measure_cpu/EP_omp32/cpu$v.txt};" >> out.txt
done