#!/usr/bin/env bash

# expected arguments :
# - which C++ standard to use : 98, 17, ...
# - which level of optimization : 0, 1, 2, ...
# - which program to make : particules-oo, particules-11, ...

prog=${1}
shift

# warm-up
${UNIT_BUILD}/${prog}.exe $* >> /dev/null

# timing
rm -f ${UNIT_BUILD}/${prog}.log
for i in 0 1 2 3 4 5 6 7 8 9 10; do
  ( time ${UNIT_BUILD}/${prog}.exe $* ) >> ${UNIT_BUILD}/${prog}.log 2>&1
done

median-user-time.py ${UNIT_BUILD}/${prog}.log $*
