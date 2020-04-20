#!/bin/bash

mpicc main.c -o convImage
mpirun -np 1 ./convImage