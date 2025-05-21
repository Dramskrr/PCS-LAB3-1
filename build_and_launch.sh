#!/bin/bash
#module load openmpi
mpicc main.c -o main

export ARRAY_SIZE=4000000
export RUNS=10
export CORES=1

time mpirun -np $CORES ./main