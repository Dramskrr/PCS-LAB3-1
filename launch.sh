#!/bin/bash
#module load openmpi

export ARRAY_SIZE=40000000
export RUNS=10
export CORES=2

time mpirun -np $CORES ./main