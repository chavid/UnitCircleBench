#!/usr/bin/env bash

# compilation
std=20
size=1024
degree=1000000
prog=power3

# O0
echo
echo ${prog} -O0
dev-gcc compile.py g++ c++${std} O0 ${prog}

dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} raw
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} alg
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} seq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} unseq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O1
echo
echo ${prog} -O1
dev-gcc compile.py g++ c++${std} O1 ${prog}

dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} raw
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} alg
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} seq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} unseq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O2
echo
echo ${prog} -O2
dev-gcc compile.py g++ c++${std} O2 ${prog}

dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} raw
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} alg
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} seq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} unseq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# O3
echo
echo ${prog} -O3
dev-gcc compile.py g++ c++${std} O3 ${prog}

dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} raw
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} alg
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} seq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} unseq
dev-gcc run-with-times.py x11 ${prog} ${size} ${degree} par_unseq

# fin
echo