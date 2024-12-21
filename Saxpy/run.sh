#!/usr/bin/env bash

# expected arguments :
# - which C++ standard to use : 98, 17, ...
# - which level of optimization : 0, 1, 2, ...
# - which program to make : particules-oo, particules-11, ...

prog=${1}
shift

rm -f tmp.${prog}.log
echo "---" >> tmp.${prog}.log
for i in 0 1 2 3 4 5 6 7 8 9 ; do
  ( \time -f "GNU time: %U s, %M kBytes." ./tmp.${prog}.exe $* ) >> ./tmp.${prog}.log 2>&1
  echo "---" >> tmp.${prog}.log
done

./mean-user-time.py ./tmp.${prog}.log ${prog} $*
