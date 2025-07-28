#!/usr/bin/env bash

## vector

echo
cpp20 compile.py g++ c++23 O3 complexes 
cpp20 run-with-times.py x11 complexes aos vector double 1024 1000000
cpp20 run-with-times.py x11 complexes soa vector double 1024 1000000

## kiwaku

echo
cpp20 compile.py g++ c++23 O3 kiwaku 
cpp20 run-with-times.py x11 kiwaku aos double 1024 1000000
cpp20 run-with-times.py x11 kiwaku soa double 1024 1000000

## End

echo

