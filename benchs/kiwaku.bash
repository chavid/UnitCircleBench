##!/usr/bin/env bash

### vector
#
#echo
#dev-clang compile.py clang++ c++23 O3 complexes 
#dev-clang run-with-times.py x11 complexes aos vector double 1024 100000
#dev-clang run-with-times.py x11 complexes soa vector double 1024 100000
#
#### valarray
#
#echo
#dev-clang run-with-times.py x11 complexes aos valarray double 1024 100000
#dev-clang run-with-times.py x11 complexes soa valarray double 1024 100000

## kiwaku

#echo
#kwk compile.py g++ c++23 O3 kiwaku 
#kwk run-with-times.py x11 kiwaku aos double 1024 1000000
#kwk run-with-times.py x11 kiwaku cpu soa double 1024 1000000
#kwk run-with-times.py x11 kiwaku simd soa double 1024 1000000

## kiwaku sycl

echo
kwk-icpx compile.py icpx c++23 O3 kiwaku-sycl
#kwk-icpx run-with-times.py x11 kiwaku cpu soa double 1024 1000000
#kwk-icpx run-with-times.py x11 kiwaku simd soa double 1024 1000000
kwk-icpx run-with-times.py x11 kiwaku-sycl aos double 1024 1000
kwk-icpx run-with-times.py x11 kiwaku-sycl soa double 1024 1000

## End

echo

