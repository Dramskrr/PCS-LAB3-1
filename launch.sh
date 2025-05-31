#!/bin/bash

export ARRAY_SIZE_CONFIG=2000000
export RUNS_CONFIG=10
export HW_THREADS_CONFIG=1

if [ $HW_THREADS_CONFIG -lt 2 ]; then
    ./serial.sh
else
    sed -i "3,+0 s|.*|export ARRAY_SIZE=$ARRAY_SIZE_CONFIG|g" parallel.lsf
    sed -i "4,+0 s|.*|export RUNS=$RUNS_CONFIG|g" parallel.lsf
    sed -i "5,+0 s|.*|export HW_THREADS=$HW_THREADS_CONFIG|g" parallel.lsf

    if ! command -v bsub > /dev/null; then
        echo "Не получилось найти через BSUB, запускаем напрямую"
        ./parallel.lsf
    else
        bsub < ./parallel.lsf
    fi
    
fi