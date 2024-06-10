#!/usr/bin/env bash

./compile-docker.sh 17 2 saxpy-soa

# tour de chauffe
./run-docker.sh saxpy-soa float 1024 1000000
./run-docker.sh saxpy-soa double 1024 1000000
./run-docker.sh saxpy-soa long 1024 1000000

# tour de confirmation
./run-docker.sh saxpy-soa float 1024 1000000
./run-docker.sh saxpy-soa double 1024 1000000
./run-docker.sh saxpy-soa long 1024 1000000
