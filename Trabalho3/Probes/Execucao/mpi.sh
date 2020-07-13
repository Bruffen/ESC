#!/bin/sh

folder="../tp3_implementations/OpenMPI/2_MPI"
image="../tp3_implementations"

export PATH=/usr/local/bin:$PATH

cd "${folder}"
mpicc -o distance_transform distance_transform.c
cd $OLDPWD

#dtrace -q -s sysc.d -c "${folder}/distance_transform "${image}/img1024.pgm" 1 4"

for th in 2 4 8 12   
do
	echo "-----------------Thread size ${th}----------------------"	

	for size in 256 512 1024 2048 4096
	do
		echo "----------------Image size ${size}-----------------------"

		#dtrace -q -s functime.d -c "${folder}/distance_transform "${image}/img1024.pgm"" 
#distance_transform

		mpirun -np ${th} --mca btl ^openib dtrace -q -s functhreadtime.d -c "${folder}/distance_transform "${image}/img${size}.pgm"" distance_transform

		mpirun -np ${th} --mca btl ^openib dtrace -q -s mpitrace.d -c "${folder}/distance_transform "${image}/img${size}.pgm""

		
		mpirun -np ${th} --mca btl ^openib dtrace -q -s lockstattime.d -c "${folder}/distance_transform "${image}/img${size}.pgm""

		#dtrace -s proctimes.d -c "${folder}/distance_transform "${image}/img1024.pgm" "

	done
done


