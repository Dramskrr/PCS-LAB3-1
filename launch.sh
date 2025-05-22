#!/bin/bash
#module load openmpi

export ARRAY_SIZE=100000000
export RUNS=10
export CORES=4

time mpirun -np $CORES ./main