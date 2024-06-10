#!/usr/bin/env bash

# common parameters
opt=0
size=1024
repeat=100000

# sans if
./compile-docker.sh 17 ${opt} saxpy
./compile-docker.sh 17 ${opt} saxpy-divide
./compile-docker.sh 17 ${opt} saxpy-virtual
./compile-docker.sh 17 ${opt} saxpy-divide-virtual
./run-docker.sh saxpy ${size} ${repeat}
./run-docker.sh saxpy-divide ${size} ${repeat}
./run-docker.sh saxpy-virtual ${size} ${repeat}
./run-docker.sh saxpy-divide-virtual ${size} ${repeat}

# avec if
./compile-docker.sh 17 ${opt} saxpy-if
./compile-docker.sh 17 ${opt} saxpy-if-divide
./compile-docker.sh 17 ${opt} saxpy-if-virtual
./compile-docker.sh 17 ${opt} saxpy-if-divide-virtual
./run-docker.sh saxpy-if ${size} ${repeat}
./run-docker.sh saxpy-if-divide ${size} ${repeat}
./run-docker.sh saxpy-if-virtual ${size} ${repeat}
./run-docker.sh saxpy-if-divide-virtual ${size} ${repeat}

# avec booleen
./compile-docker.sh 17 ${opt} saxpy-bool
./compile-docker.sh 17 ${opt} saxpy-bool-divide
./compile-docker.sh 17 ${opt} saxpy-bool-virtual
./compile-docker.sh 17 ${opt} saxpy-bool-divide-virtual
./run-docker.sh saxpy-bool ${size} ${repeat}
./run-docker.sh saxpy-bool-divide ${size} ${repeat}
./run-docker.sh saxpy-bool-virtual ${size} ${repeat}
./run-docker.sh saxpy-bool-divide-virtual ${size} ${repeat}
