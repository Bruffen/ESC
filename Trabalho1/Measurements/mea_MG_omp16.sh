#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=0:05:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

cd "ESC/T1/NPB3.3.1/NPB3.3-OMP"
cd config/
cp make.def.use make.def

echo -n "" > suite.def
echo MG B >> suite.def

cd ..
make clean
make suite
chmod +x ./bin/*

cd ~
cd ESC/T1/Measurements/omp

export OMP_NUM_THREADS=16
"../../NPB3.3.1/NPB3.3-OMP/bin/mg.B.x" &
timeout 30 iostat -m 1 >> "io_MG_omp16.txt" &
timeout 30 vmstat -S M 1 >> "vm_MG_omp16.txt" &
timeout 30 mpstat -P ON 1 >> "mp_MG_omp16.txt" &

sleep 30