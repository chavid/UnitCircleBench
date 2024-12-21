#!/usr/bin/env bash

# expected arguments :
# - which C++ standard to use : 98, 17, ...
# - which level of optimization : 0, 1, 2, ...
# - which program to make : particules-oo, particules-11, ...

std=${1}
shift
opt=${1}
shift
prog=${1}
shift

rm -f tmp.${prog}.exe
g++ -g -march=native -mtune=native -Wall -Wextra -Wfatal-errors -std=c++${std} -O${opt} ${prog}.cpp -o tmp.${prog}.exe

if [ $? -ne 0 ]; then
  echo G++ FAILED
  exit 1
fi
