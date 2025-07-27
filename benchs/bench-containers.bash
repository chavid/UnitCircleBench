#!/usr/bin/env bash

# Less repetitions, because of -O0 and std::list

cpp20 compile.py g++ c++23 O0 axpy

cpp20 run-with-times.py x11 axpy aos carray   double 1024 200000
cpp20 run-with-times.py x11 axpy aos dynarray double 1024 200000
cpp20 run-with-times.py x11 axpy aos array    double 1024 200000
cpp20 run-with-times.py x11 axpy aos valarray double 1024 200000
cpp20 run-with-times.py x11 axpy aos vector   double 1024 200000
cpp20 run-with-times.py x11 axpy aos list     double 1024 200000

cpp20 run-with-times.py x11 axpy soa carray   double 1024 200000
cpp20 run-with-times.py x11 axpy soa dynarray double 1024 200000
cpp20 run-with-times.py x11 axpy soa array    double 1024 200000
cpp20 run-with-times.py x11 axpy soa valarray double 1024 200000
cpp20 run-with-times.py x11 axpy soa vector   double 1024 200000
cpp20 run-with-times.py x11 axpy soa list     double 1024 200000

# Same with -O2

cpp20 ./compile.py g++ c++23 O2 axpy

cpp20 run-with-times.py x11 axpy aos carray   double 1024 200000
cpp20 run-with-times.py x11 axpy aos dynarray double 1024 200000
cpp20 run-with-times.py x11 axpy aos array    double 1024 200000
cpp20 run-with-times.py x11 axpy aos valarray double 1024 200000
cpp20 run-with-times.py x11 axpy aos vector   double 1024 200000
cpp20 run-with-times.py x11 axpy aos list     double 1024 200000

cpp20 run-with-times.py x11 axpy soa carray   double 1024 200000
cpp20 run-with-times.py x11 axpy soa dynarray double 1024 200000
cpp20 run-with-times.py x11 axpy soa array    double 1024 200000
cpp20 run-with-times.py x11 axpy soa valarray double 1024 200000
cpp20 run-with-times.py x11 axpy soa vector   double 1024 200000
cpp20 run-with-times.py x11 axpy soa list     double 1024 200000

# CPU-bound ?
# Move up to 1024*2000000
# I remove std::list, too slow.

cpp20 ./compile.py g++ c++23 O2 axpy

cpp20 run-with-times.py x11 axpy aos carray   double 1024 2000000
cpp20 run-with-times.py x11 axpy aos dynarray double 1024 2000000
cpp20 run-with-times.py x11 axpy aos array    double 1024 2000000
cpp20 run-with-times.py x11 axpy aos valarray double 1024 2000000
cpp20 run-with-times.py x11 axpy aos vector   double 1024 2000000

cpp20 run-with-times.py x11 axpy soa carray   double 1024 2000000
cpp20 run-with-times.py x11 axpy soa dynarray double 1024 2000000
cpp20 run-with-times.py x11 axpy soa array    double 1024 2000000
cpp20 run-with-times.py x11 axpy soa valarray double 1024 2000000
cpp20 run-with-times.py x11 axpy soa vector   double 1024 2000000

# Same with -O3

cpp20 ./compile.py g++ c++23 O3 axpy

cpp20 run-with-times.py x11 axpy aos carray   double 1024 2000000
cpp20 run-with-times.py x11 axpy aos dynarray double 1024 2000000
cpp20 run-with-times.py x11 axpy aos array    double 1024 2000000
cpp20 run-with-times.py x11 axpy aos valarray double 1024 2000000
cpp20 run-with-times.py x11 axpy aos vector   double 1024 2000000

cpp20 run-with-times.py x11 axpy soa carray   double 1024 2000000
cpp20 run-with-times.py x11 axpy soa dynarray double 1024 2000000
cpp20 run-with-times.py x11 axpy soa array    double 1024 2000000
cpp20 run-with-times.py x11 axpy soa valarray double 1024 2000000
cpp20 run-with-times.py x11 axpy soa vector   double 1024 2000000

# IO/bound ?
# Move to 65536*31250

cpp20 ./compile.py g++ c++23 O3 axpy

cpp20 run-with-times.py x11 axpy aos carray   double 65536 31250
cpp20 run-with-times.py x11 axpy aos dynarray double 65536 31250
cpp20 run-with-times.py x11 axpy aos array    double 65536 31250
cpp20 run-with-times.py x11 axpy aos valarray double 65536 31250
cpp20 run-with-times.py x11 axpy aos vector   double 65536 31250

cpp20 run-with-times.py x11 axpy soa carray   double 65536 31250
cpp20 run-with-times.py x11 axpy soa dynarray double 65536 31250
cpp20 run-with-times.py x11 axpy soa array    double 65536 31250
cpp20 run-with-times.py x11 axpy soa valarray double 65536 31250
cpp20 run-with-times.py x11 axpy soa vector   double 65536 31250

