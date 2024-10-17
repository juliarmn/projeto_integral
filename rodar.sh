#!/bin/bash

THREADS=(1 2 4 8)
INTERVALS_X=(1000 10000 100000)
INTERVALS_Y=(1000 10000 100000)

for threads in "${THREADS[@]}"; do
    for n_intervals_x in "${INTERVALS_X[@]}"; do
        for n_intervals_y in "${INTERVALS_Y[@]}"; do
            echo "Executando com OMP_NUM_THREADS=${threads}, n_intervals_x=${n_intervals_x}, n_intervals_y=${n_intervals_y}"
            OMP_NUM_THREADS=$threads ./p $n_intervals_x $n_intervals_y
        done
    done
done

