##!/usr/bin/env bash

clear
docker run --gpus all --device=/dev/dri --user $(id -u):$(id -g) --rm -v "${PWD}":/work -w /work -it chavid/kiwaku-icpx:2025.2-bookworm bash
source /opt/intel/oneapi/setvars.sh
source env.bash
compile.py icpx c++23 O3 kwk-sycl-gpu 
time build/kwk-sycl-gpu.exe 1024 100
