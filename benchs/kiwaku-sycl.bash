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

## kiwaku sycl

echo
kwk-icpx compile.py icpx c++23 O3 kiwaku-sycl

kwk-icpx run-with-times.py x11 kiwaku-sycl cpu aos double 1024 10000
kwk-icpx run-with-times.py x11 kiwaku-sycl cpu soa double 1024 10000

kwk-icpx run-with-times.py x11 kiwaku-sycl gpu aos double 1024 10000
kwk-icpx run-with-times.py x11 kiwaku-sycl gpu soa double 1024 10000

## End

echo

