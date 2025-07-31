##!/usr/bin/env bash

## vector

#echo
#cpp20 compile.py g++ c++23 O3 complexes 
##cpp20 run-with-times.py x11 complexes aos vector double 1024 1000000
#cpp20 run-with-times.py x11 complexes soa vector double 1024 1000000

### valarray
#
#echo
##cpp20 run-with-times.py x11 complexes aos vector double 1024 1000000
#cpp20 run-with-times.py x11 complexes soa valarray double 1024 1000000

## kiwaku

#echo
#kwk compile.py g++ c++23 O3 kiwaku 
#kwk run-with-times.py x11 kiwaku aos double 1024 1000000
#kwk run-with-times.py x11 kiwaku cpu soa double 1024 1000000
#kwk run-with-times.py x11 kiwaku simd soa double 1024 1000000

## kiwaku sycl

echo
kwk-icpx compile.py icpx c++23 O3 kiwaku 
#kwk-icpx run-with-times.py x11 kiwaku aos double 1024 1000000
#kwk-icpx run-with-times.py x11 kiwaku cpu soa double 1024 1000000
#kwk-icpx run-with-times.py x11 kiwaku simd soa double 1024 1000000
kwk-icpx run-with-times.py x11 kiwaku sycl soa double 1024 1000000

## End

echo

