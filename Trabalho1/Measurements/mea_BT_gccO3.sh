#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=0:20:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

cd "ESC/T1/NPB3.3.1/NPB3.3-SER"
cd config/
cp make.def.gccO3 make.def

echo -n "" > suite.def
echo BT B >> suite.def

cd ..
make clean
make suite
chmod +x ./bin/*

cd ~
cd ESC/T1/Measurements/gccO3

"../../NPB3.3.1/NPB3.3-SER/bin/bt.B.x" &
timeout 300 iostat -m 4 >> "io_BT_gccO3.txt" &
timeout 300 vmstat -S M 4 >> "vm_BT_gccO3.txt" &
timeout 300 mpstat -P ON 4 >> "mp_BT_gccO3.txt" &

sleep 300