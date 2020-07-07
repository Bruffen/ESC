#!/bin/sh

make
./distance_transform "../img${1}.pgm" > "./results${1}.txt"
