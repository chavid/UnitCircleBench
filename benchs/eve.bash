##!/usr/bin/env bash

eve-gcc compile.py g++ c++23 O3 eve 

eve-gcc run-with-times.py x11 eve raw double 1024 100000
eve-gcc run-with-times.py x11 eve simd double 1024 100000
eve-gcc run-with-times.py x11 eve wide double 1024 100000
eve-gcc run-with-times.py x11 eve transform double 1024 100000

