#!/bin/bash

mpicc main.c -o convImage -lm
mpirun -np 4 ./convImage