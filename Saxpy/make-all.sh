#!/usr/bin/env bash

# fast : 256*100000
# io-bound ? 16384*100000
# compute-bound ? 1024*1000000

./docker.sh ./compile.sh 23 3 saxpy

./docker.sh ./run.sh saxpy aos valarray half 256 100000
./docker.sh ./run.sh saxpy aos valarray float 256 100000
./docker.sh ./run.sh saxpy aos valarray double 256 100000
./docker.sh ./run.sh saxpy aos valarray long 256 100000
./docker.sh ./run.sh saxpy aos valarray quad 256 100000

./docker.sh ./run.sh saxpy aos dynarray double 256 100000
./docker.sh ./run.sh saxpy aos valarray double 256 100000
./docker.sh ./run.sh saxpy aos vector double 256 100000

./docker.sh ./run.sh saxpy soa valarray half 256 100000
./docker.sh ./run.sh saxpy soa valarray float 256 100000
./docker.sh ./run.sh saxpy soa valarray double 256 100000
./docker.sh ./run.sh saxpy soa valarray long 256 100000
./docker.sh ./run.sh saxpy soa valarray quad 256 100000

./docker.sh ./run.sh saxpy soa dynarray double 256 100000
./docker.sh ./run.sh saxpy soa valarray double 256 100000
./docker.sh ./run.sh saxpy soa vector double 256 100000

