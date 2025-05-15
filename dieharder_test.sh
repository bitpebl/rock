#!/bin/sh

gcc -O3 -march=native -mtune=native main.c rock.c -o rockprng_test
./rockprng_test | dieharder -a -g 200 | tee dieharder_out.txt
