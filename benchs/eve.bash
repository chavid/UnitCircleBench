##!/usr/bin/env bash

kwk-gcc compile.py g++ c++23 O3 eve 

kwk-gcc run-with-times.py x11 eve raw double 1024 100000
kwk-gcc run-with-times.py x11 eve wide double 1024 100000
kwk-gcc run-with-times.py x11 eve transform double 1024 100000

