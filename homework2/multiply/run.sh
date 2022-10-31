#!/bin/bash
gcc multiply.c -fopenmp -mavx2 -o multiply
gcc check.c -o check

./multiply
./check
