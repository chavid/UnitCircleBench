#!/usr/bin/env bash

# compilation
std=20
size=1024
degree=1000000
prog=power3

# O0
echo
echo ${prog} -O0
cpp20 compile.py g++ c++${std} O0 ${prog}

cpp20 run-with-times.py x11 ${prog} ${size} ${degree} raw
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} alg
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} seq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} unseq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O1
echo
echo ${prog} -O1
cpp20 compile.py g++ c++${std} O1 ${prog}

cpp20 run-with-times.py x11 ${prog} ${size} ${degree} raw
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} alg
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} seq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} unseq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O2
echo
echo ${prog} -O2
cpp20 compile.py g++ c++${std} O2 ${prog}

cpp20 run-with-times.py x11 ${prog} ${size} ${degree} raw
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} alg
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} seq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} unseq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O3
echo
echo ${prog} -O3
cpp20 compile.py g++ c++${std} O3 ${prog}

cpp20 run-with-times.py x11 ${prog} ${size} ${degree} raw
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} alg
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} seq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} unseq
cpp20 run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# fin
echo