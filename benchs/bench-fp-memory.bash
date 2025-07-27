#!/usr/bin/env bash

cpp20 compile.py g++ c++23 O3 axpy

cpp20 run-with-times.py x11 axpy aos vector half   65536 3125
cpp20 run-with-times.py x11 axpy aos vector float  65536 3125
cpp20 run-with-times.py x11 axpy aos vector double 65536 3125
cpp20 run-with-times.py x11 axpy aos vector long   65536 3125
cpp20 run-with-times.py x11 axpy aos vector quad   65536 3125

cpp20 run-with-times.py x11 axpy soa vector half   65536 3125
cpp20 run-with-times.py x11 axpy soa vector float  65536 3125
cpp20 run-with-times.py x11 axpy soa vector double 65536 3125
cpp20 run-with-times.py x11 axpy soa vector long   65536 3125
cpp20 run-with-times.py x11 axpy soa vector quad   65536 3125


