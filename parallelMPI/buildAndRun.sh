#!/bin/bash

mpicc main.c -o convImage -lm
mpirun -np 5 ./convImage