#!/bin/sh

#PBS -N "npb"
#PBS -l walltime=0:10:00
#PBS -l nodes=4:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0

cd "ESC/T1/NPB3.3.1/NPB3.3-MPI"
cd config/
cp make.def.use make.def

echo -n "" > suite.def
echo BT B 64 >> suite.def

cd ..
make clean
make suite
chmod +x ./bin/*

cd ~
cd ESC/T1/Measurements/mpi

mpirun -np 64 -mca btl self,sm,tcp -oversubscribe --map-by core "../../NPB3.3.1/NPB3.3-OMP/bin/bt.B.64" &
timeout 150 iostat -m 1 >> "io_BT_omp16.txt" &
timeout 150 vmstat -S M 1 >> "vm_BT_omp16.txt" &
timeout 150 mpstat -P ON 1 >> "mp_BT_omp16.txt" &

sleep 150