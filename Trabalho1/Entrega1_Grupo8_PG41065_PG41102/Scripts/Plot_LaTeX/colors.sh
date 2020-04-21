#!/bin/bash
#Script to generate colors for the multiple lines and files for LaTeX accordingly

step=6
curr=0

for v in {0..31}
do
    s=$(( $v*$step ))
    echo "\addplot [color={rgb,255:red,$s;green,255;blue,$s}, mark=*] table {Data/measure_cpu/MG_omp16/cpu$v.txt};" >> out.txt
done
