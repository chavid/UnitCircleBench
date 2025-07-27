#!/usr/bin/env bash

cpp20 compile.py g++ c++23 O3 complexes

cpp20 run-with-times.py x11 complexes aos vector   half   1024 100000
cpp20 run-with-times.py x11 complexes aos vector   float  1024 100000
cpp20 run-with-times.py x11 complexes aos vector   double 1024 100000
cpp20 run-with-times.py x11 complexes aos vector   long   1024 100000
cpp20 run-with-times.py x11 complexes aos vector   quad   1024 100000

