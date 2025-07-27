#!/usr/bin/env bash

## Less repetitions, because of -O0 and std::list
#
#cpp20 compile.py g++ c++23 O0 complexes
#
#cpp20 run-with-times.py x11 complexes aos carray   float  1024 10000
#cpp20 run-with-times.py x11 complexes aos dynarray float  1024 10000
#cpp20 run-with-times.py x11 complexes aos array    float  1024 10000
#cpp20 run-with-times.py x11 complexes aos valarray float  1024 10000
#cpp20 run-with-times.py x11 complexes aos vector   float  1024 10000
#cpp20 run-with-times.py x11 complexes aos list     float  1024 10000
#
#cpp20 run-with-times.py x11 complexes soa carray   float  1024 10000
#cpp20 run-with-times.py x11 complexes soa dynarray float  1024 10000
#cpp20 run-with-times.py x11 complexes soa array    float  1024 10000
#cpp20 run-with-times.py x11 complexes soa valarray float  1024 10000
#cpp20 run-with-times.py x11 complexes soa vector   float  1024 10000
#cpp20 run-with-times.py x11 complexes soa list     float  1024 10000
#
## Same with -O2
#
#cpp20 compile.py g++ c++23 O2 complexes
#
#cpp20 run-with-times.py x11 complexes aos carray   float  1024 10000
#cpp20 run-with-times.py x11 complexes aos dynarray float  1024 10000
#cpp20 run-with-times.py x11 complexes aos array    float  1024 10000
#cpp20 run-with-times.py x11 complexes aos valarray float  1024 10000
#cpp20 run-with-times.py x11 complexes aos vector   float  1024 10000
#cpp20 run-with-times.py x11 complexes aos list     float  1024 10000
#
#cpp20 run-with-times.py x11 complexes soa carray   float  1024 10000
#cpp20 run-with-times.py x11 complexes soa dynarray float  1024 10000
#cpp20 run-with-times.py x11 complexes soa array    float  1024 10000
#cpp20 run-with-times.py x11 complexes soa valarray float  1024 10000
#cpp20 run-with-times.py x11 complexes soa vector   float  1024 10000
#cpp20 run-with-times.py x11 complexes soa list     float  1024 10000

# CPU-bound ?
# Move up to 1024*100000
# I remove std::list, too slow.

#cpp20 compile.py g++ c++23 O2 complexes
#
#cpp20 run-with-times.py x11 complexes aos carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes aos dynarray float  1024 100000
#cpp20 run-with-times.py x11 complexes aos array    float  1024 100000
#cpp20 run-with-times.py x11 complexes aos valarray float  1024 100000
#cpp20 run-with-times.py x11 complexes aos vector   float  1024 100000
#
#cpp20 run-with-times.py x11 complexes soa carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes soa dynarray float  1024 100000
#cpp20 run-with-times.py x11 complexes soa array    float  1024 100000
#cpp20 run-with-times.py x11 complexes soa valarray float  1024 100000
#cpp20 run-with-times.py x11 complexes soa vector   float  1024 100000

# Same with -O3

cpp20 compile.py g++ c++23 O3 complexes

#cpp20 run-with-times.py x11 complexes aos carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes aos dynarray float  1024 100000
#cpp20 run-with-times.py x11 complexes aos array    float  1024 100000
#cpp20 run-with-times.py x11 complexes aos valarray float  1024 100000
#cpp20 run-with-times.py x11 complexes aos vector   float  1024 100000

cpp20 run-with-times.py x11 complexes soa carray   float  1024 100000
cpp20 run-with-times.py x11 complexes soa dynarray float  1024 100000
#cpp20 run-with-times.py x11 complexes soa array    float  1024 100000
#cpp20 run-with-times.py x11 complexes soa valarray float  1024 100000
#cpp20 run-with-times.py x11 complexes soa vector   float  1024 100000

## More I/O bound
#
#cpp20 compile.py g++ c++23 O3 complexes
#
#cpp20 run-with-times.py x11 complexes aos carray   float  65536 32768
#cpp20 run-with-times.py x11 complexes aos dynarray float  65536 32768
#cpp20 run-with-times.py x11 complexes aos array    float  65536 32768
#cpp20 run-with-times.py x11 complexes aos valarray float  65536 32768
#cpp20 run-with-times.py x11 complexes aos vector   float  65536 32768
#
#cpp20 run-with-times.py x11 complexes soa carray   float  65536 32768
#cpp20 run-with-times.py x11 complexes soa dynarray float  65536 32768
#cpp20 run-with-times.py x11 complexes soa array    float  65536 32768
#cpp20 run-with-times.py x11 complexes soa valarray float  65536 32768
#cpp20 run-with-times.py x11 complexes soa vector   float  65536 32768

