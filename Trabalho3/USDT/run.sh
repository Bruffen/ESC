#!/bin/sh

dtrace -o probes.h -h -s dtprovider.d
make
dtrace -G -s dtprovider.d distance_transform -o distance_t.o

gcc -Wl,--export-dynamic,--strip-all -fopenmp -o distance_t distance_t.o distance_transform probes.h -lm
