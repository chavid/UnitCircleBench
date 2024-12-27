#!/usr/bin/env bash

# -O0, 1024*200000

./docker-devcpp20.sh  ./compile-g++.sh 23 0 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh  ./run.sh saxpy aos vector half   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector float  1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector double 1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector long   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector quad   1024 200000

./docker-devcpp20.sh  ./run.sh saxpy soa vector half   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector float  1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector double 1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector long   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector quad   1024 200000

# Same with -O2

./docker-devcpp20.sh  ./compile-g++.sh 23 2 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh  ./run.sh saxpy aos vector half   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector float  1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector double 1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector long   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy aos vector quad   1024 200000

./docker-devcpp20.sh  ./run.sh saxpy soa vector half   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector float  1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector double 1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector long   1024 200000
./docker-devcpp20.sh  ./run.sh saxpy soa vector quad   1024 200000

# Compute-bound ? 1024 2000000

./docker-devcpp20.sh  ./run.sh saxpy aos vector half   1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector float  1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector double 1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector long   1024 2000000

./docker-devcpp20.sh  ./run.sh saxpy soa vector half   1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector float  1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector double 1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector long   1024 2000000

# -O3

./docker-devcpp20.sh  ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh  ./run.sh saxpy aos vector half   1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector float  1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector double 1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy aos vector long   1024 2000000

./docker-devcpp20.sh  ./run.sh saxpy soa vector half   1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector float  1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector double 1024 2000000
./docker-devcpp20.sh  ./run.sh saxpy soa vector long   1024 2000000


