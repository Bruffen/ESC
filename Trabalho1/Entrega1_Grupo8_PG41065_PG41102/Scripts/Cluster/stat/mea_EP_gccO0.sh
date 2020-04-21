#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=0:05:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

cd "ESC/T1/NPB3.3.1/NPB3.3-SER"
cd config/
cp make.def.gccO0 make.def

echo -n "" > suite.def
echo EP B >> suite.def

cd ..
make clean
make suite
chmod +x ./bin/*

cd ~
cd ESC/T1/Measurements/gccO0

"../../NPB3.3.1/NPB3.3-SER/bin/ep.B.x" &
timeout 180 iostat -m 2 >> "io_EP_gccO0.txt" &
timeout 180 vmstat -S M 2 >> "vm_EP_gccO0.txt" &
timeout 180 mpstat -P ON 2 >> "mp_EP_gccO0.txt" &

sleep 180