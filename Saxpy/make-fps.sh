#!/usr/bin/env bash

./docker.sh ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

# fast : 256*100000

./docker.sh ./run.sh saxpy aos valarray half   256 100000
./docker.sh ./run.sh saxpy aos valarray float  256 100000
./docker.sh ./run.sh saxpy aos valarray double 256 100000
./docker.sh ./run.sh saxpy aos valarray long   256 100000
./docker.sh ./run.sh saxpy aos valarray quad   256 100000

./docker.sh ./run.sh saxpy soa valarray half   256 100000
./docker.sh ./run.sh saxpy soa valarray float  256 100000
./docker.sh ./run.sh saxpy soa valarray double 256 100000
./docker.sh ./run.sh saxpy soa valarray long   256 100000
./docker.sh ./run.sh saxpy soa valarray quad   256 100000


# io-bound ? 16384 100000

./docker.sh ./run.sh saxpy aos valarray half   16384 100000
./docker.sh ./run.sh saxpy aos valarray float  16384 100000
./docker.sh ./run.sh saxpy aos valarray double 16384 100000
./docker.sh ./run.sh saxpy aos valarray long   16384 100000
./docker.sh ./run.sh saxpy aos valarray quad   16384 100000

./docker.sh ./run.sh saxpy soa valarray half   16384 100000
./docker.sh ./run.sh saxpy soa valarray float  16384 100000
./docker.sh ./run.sh saxpy soa valarray double 16384 100000
./docker.sh ./run.sh saxpy soa valarray long   16384 100000
./docker.sh ./run.sh saxpy soa valarray quad   16384 100000


# compute-bound ? 1024*1000000

