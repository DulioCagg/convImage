#!/bin/bash
# Modify image name inside main.c before running c:

for core in {2..8};
do
    mpicc main.c -o convImage -lm
    for time in {1..10};
    do
        mpirun -np $core ./convImage >> results.txt
    done
done
