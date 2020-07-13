#!/bin/sh

folder="../tp3_implementations/2_Parallel_Dynamic"
image="../tp3_implementations"

cd "${folder}"
make
cd $OLDPWD

dtrace -q -s sysc.d -c "${folder}/distance_transform "${image}/img1024.pgm" 1 4"

for th in 1 2 4 8 12
do
	echo "-----------------Thread size ${th}----------------------"	

	for size in 256 512 1024 2048 4096
	do
		echo "----------------Image size ${size}-----------------------"

		dtrace -q -s functime.d -c "${folder}/distance_transform "${image}/img1024.pgm"" distance_transform

		dtrace -q -s functhreadtime.d -c "${folder}/distance_transform "${image}/img${size}.pgm" ${th} 4" distance_transform

		dtrace -q -s lockstattime.d -c "${folder}/distance_transform "${image}/img${size}.pgm" ${th} 4"

		#dtrace -s proctimes.d -c "${folder}/distance_transform "${image}/img1024.pgm" "

	done
done

