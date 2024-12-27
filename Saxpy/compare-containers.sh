#!/usr/bin/env bash

# Less repetitions, because of -O0 and std::list

./docker-devcpp20.sh ./compile-g++.sh 23 0 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos carray   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos dynarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos array    double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos valarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos vector   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos list     double 1024 200000

./docker-devcpp20.sh ./run.sh saxpy soa carray   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa dynarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa array    double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa valarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa vector   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa list     double 1024 200000

# Same with -O2

./docker-devcpp20.sh ./compile-g++.sh 23 2 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos carray   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos dynarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos array    double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos valarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos vector   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy aos list     double 1024 200000

./docker-devcpp20.sh ./run.sh saxpy soa carray   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa dynarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa array    double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa valarray double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa vector   double 1024 200000
./docker-devcpp20.sh ./run.sh saxpy soa list     double 1024 200000

# CPU-bound ?
# Move up to 1024*2000000
# I remove std::list, too slow.

./docker-devcpp20.sh ./compile-g++.sh 23 2 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos carray   double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos dynarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos array    double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos valarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos vector   double 1024 2000000

./docker-devcpp20.sh ./run.sh saxpy soa carray   double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa dynarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa array    double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa valarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa vector   double 1024 2000000

# Same with -O3

./docker-devcpp20.sh ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos carray   double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos dynarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos array    double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos valarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy aos vector   double 1024 2000000

./docker-devcpp20.sh ./run.sh saxpy soa carray   double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa dynarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa array    double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa valarray double 1024 2000000
./docker-devcpp20.sh ./run.sh saxpy soa vector   double 1024 2000000


# IO/bound ?
# Move to 65536*31250


./docker-devcpp20.sh ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos carray   double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy aos dynarray double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy aos array    double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy aos valarray double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy aos vector   double 65536 31250

./docker-devcpp20.sh ./run.sh saxpy soa carray   double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy soa dynarray double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy soa array    double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy soa valarray double 65536 31250
./docker-devcpp20.sh ./run.sh saxpy soa vector   double 65536 31250

