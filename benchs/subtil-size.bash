#!/usr/bin/env bash

cpp20 compile.py g++ c++23 O3 subtil-size
cpp20 run-with-times.py x11 subtil-size c1 float 1024 100000
cpp20 run-with-times.py x11 subtil-size c2 float 1024 100000
