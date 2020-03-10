#!/bin/bash

rm results.txt
gcc -fopenmp main.c -o convImage -lm
