#!/usr/bin/env bash

# compilation
std=20
size=1024
degree=1000000
prog=power3

# O0
echo
echo ${prog} -O0
./compile.sh ${std} 0 ${prog}
./run.sh ${prog} ${size} ${degree} raw
./run.sh ${prog} ${size} ${degree} alg
./run.sh ${prog} ${size} ${degree} seq
./run.sh ${prog} ${size} ${degree} par
./run.sh ${prog} ${size} ${degree} unseq
./run.sh ${prog} ${size} ${degree} par_unseq

# O1
echo
echo ${prog} -O1
./compile.sh ${std} 1 ${prog}
./run.sh ${prog} ${size} ${degree} raw
./run.sh ${prog} ${size} ${degree} alg
./run.sh ${prog} ${size} ${degree} seq
./run.sh ${prog} ${size} ${degree} par
./run.sh ${prog} ${size} ${degree} unseq
./run.sh ${prog} ${size} ${degree} par_unseq

# O2
echo
echo ${prog} -O2
./compile.sh ${std} 2 ${prog}
./run.sh ${prog} ${size} ${degree} raw
./run.sh ${prog} ${size} ${degree} alg
./run.sh ${prog} ${size} ${degree} seq
./run.sh ${prog} ${size} ${degree} par
./run.sh ${prog} ${size} ${degree} unseq
./run.sh ${prog} ${size} ${degree} par_unseq

# O3
echo
echo ${prog} -O3
./compile.sh ${std} 3 ${prog}
./run.sh ${prog} ${size} ${degree} raw
./run.sh ${prog} ${size} ${degree} alg
./run.sh ${prog} ${size} ${degree} seq
./run.sh ${prog} ${size} ${degree} par
./run.sh ${prog} ${size} ${degree} unseq
./run.sh ${prog} ${size} ${degree} par_unseq

# fin
echo