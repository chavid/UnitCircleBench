#!/usr/bin/env bash

dev-gcc compile.py g++ c++23 O3 subtil-size
dev-gcc run-with-times.py x11 subtil-size c1 float 1024 100000
dev-gcc run-with-times.py x11 subtil-size c2 float 1024 100000
