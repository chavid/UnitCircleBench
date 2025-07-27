#!/usr/bin/env bash

#cpp20 compile.py g++ c++23 O2 complexes-dyn
#
#cpp20 run-with-times.py x11 complexes-dyn aos carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes-dyn aos dynarray float  1024 100000
#
#cpp20 run-with-times.py x11 complexes-dyn soa carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes-dyn soa dynarray float  1024 100000

# Same with -O3

cpp20 compile.py g++ c++23 O3 complexes-dyn

#cpp20 run-with-times.py x11 complexes-dyn aos carray   float  1024 100000
#cpp20 run-with-times.py x11 complexes-dyn aos dynarray float  1024 100000

cpp20 run-with-times.py x11 complexes-dyn soa carray   float  1024 100000
cpp20 run-with-times.py x11 complexes-dyn soa dynarray float  1024 100000


