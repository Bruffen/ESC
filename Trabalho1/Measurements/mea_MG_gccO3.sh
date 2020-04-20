#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=0:05:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

cd "ESC/T1/NPB3.3.1/NPB3.3-SER"
cd config/
cp make.def.gccO3 make.def

echo -n "" > suite.def
echo MG B >> suite.def

cd ..
make clean
make suite
chmod +x ./bin/*

cd ~
cd ESC/T1/Measurements/gccO3

"../../NPB3.3.1/NPB3.3-SER/bin/mg.B.x" &
timeout 40 iostat -m 1 >> "io_MG_gccO3.txt" &
timeout 40 vmstat -S M 1 >> "vm_MG_gccO3.txt" &
timeout 40 mpstat -P ON 1 >> "mp_MG_gccO3.txt" &

sleep 40