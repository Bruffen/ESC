#!/bin/sh

for i in 1 2 3 4
do
    perf script -i ${i}c.data | ../../FlameGraph/stackcollapse-perf.pl | ../../FlameGraph/flamegraph.pl > sort${i}.svg
done