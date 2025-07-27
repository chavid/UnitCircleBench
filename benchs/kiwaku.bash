#!/usr/bin/env bash


cpp20 compile.py g++ c++23 O2 complexes
cpp20 run-with-times.py x11 complexes aos array float 1024 100000
cpp20 run-with-times.py x11 complexes soa array float 1024 100000

cpp20 compile.py g++ c++23 O3 complexes
cpp20 run-with-times.py x11 complexes aos array float 1024 100000
cpp20 run-with-times.py x11 complexes soa array float 1024 100000

# O2, 1024*100000

cpp20 compile.py g++ c++23 O2 kiwaku 
cpp20 run-with-times.py x11 kiwaku aos float 1024 100000
cpp20 run-with-times.py x11 kiwaku soa float 1024 100000

# O3, 1024*100000

cpp20 compile.py g++ c++23 O3 kiwaku 
cpp20 run-with-times.py x11 kiwaku aos float 1024 100000
cpp20 run-with-times.py x11 kiwaku soa float 1024 100000


