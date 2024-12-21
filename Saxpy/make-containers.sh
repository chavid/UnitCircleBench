#!/usr/bin/env bash

# io-bound ? 65536*100000
# compute-bound ? 1024*1000000

./docker-devcpp20.sh ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

# fast check : 1024*1000000

./docker-devcpp20.sh ./run.sh saxpy aos carray   float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy aos dynarray float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy aos array    float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy aos valarray float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy aos vector   float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy aos list     float 1024 1000000

./docker-devcpp20.sh ./run.sh saxpy soa carray   float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy soa dynarray float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy soa array    float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy soa valarray float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy soa vector   float 1024 1000000
./docker-devcpp20.sh ./run.sh saxpy soa list     float 1024 1000000

