##!/usr/bin/env bash

#kwk compile.py g++ c++23 O3 eve 

kwk run-with-times.py x11 eve raw double 1024 100000
kwk run-with-times.py x11 eve wide double 1024 100000
kwk run-with-times.py x11 eve transform double 1024 100000

# ./build/eve.exe wide double 1024 10000