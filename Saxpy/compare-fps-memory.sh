#!/usr/bin/env bash

./docker-devcpp20.sh ./compile-g++.sh 23 3 saxpy

if [ $? -ne 0 ]; then
  echo MAKE FAILED
  exit 1
fi

./docker-devcpp20.sh ./run.sh saxpy aos vector half   65536 3125
./docker-devcpp20.sh ./run.sh saxpy aos vector float  65536 3125
./docker-devcpp20.sh ./run.sh saxpy aos vector double 65536 3125
./docker-devcpp20.sh ./run.sh saxpy aos vector long   65536 3125
./docker-devcpp20.sh ./run.sh saxpy aos vector quad   65536 3125

./docker-devcpp20.sh ./run.sh saxpy soa vector half   65536 3125
./docker-devcpp20.sh ./run.sh saxpy soa vector float  65536 3125
./docker-devcpp20.sh ./run.sh saxpy soa vector double 65536 3125
./docker-devcpp20.sh ./run.sh saxpy soa vector long   65536 3125
./docker-devcpp20.sh ./run.sh saxpy soa vector quad   65536 3125


