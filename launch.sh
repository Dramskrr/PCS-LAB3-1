#!/bin/bash
#module load openmpi

export ARRAY_SIZE=100000000
export RUNS=10
export HW_THREADS=2

time mpirun --use-hwthread-cpus -np $HW_THREADS ./main