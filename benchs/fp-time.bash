#!/usr/bin/env bash

# -O0, 1024*200000

dev-gcc compile.py g++ c++23 O0 axpy

dev-gcc run-with-times.py x11 axpy aos vector half   1024 200000
dev-gcc run-with-times.py x11 axpy aos vector float  1024 200000
dev-gcc run-with-times.py x11 axpy aos vector double 1024 200000
dev-gcc run-with-times.py x11 axpy aos vector long   1024 200000
dev-gcc run-with-times.py x11 axpy aos vector quad   1024 200000

dev-gcc run-with-times.py x11 axpy soa vector half   1024 200000
dev-gcc run-with-times.py x11 axpy soa vector float  1024 200000
dev-gcc run-with-times.py x11 axpy soa vector double 1024 200000
dev-gcc run-with-times.py x11 axpy soa vector long   1024 200000
dev-gcc run-with-times.py x11 axpy soa vector quad   1024 200000

# Same with -O2

dev-gcc compile.py g++ c++23 O2 axpy

dev-gcc run-with-times.py x11 axpy aos vector half   1024 200000
dev-gcc run-with-times.py x11 axpy aos vector float  1024 200000
dev-gcc run-with-times.py x11 axpy aos vector double 1024 200000
dev-gcc run-with-times.py x11 axpy aos vector long   1024 200000
dev-gcc run-with-times.py x11 axpy aos vector quad   1024 200000

dev-gcc run-with-times.py x11 axpy soa vector half   1024 200000
dev-gcc run-with-times.py x11 axpy soa vector float  1024 200000
dev-gcc run-with-times.py x11 axpy soa vector double 1024 200000
dev-gcc run-with-times.py x11 axpy soa vector long   1024 200000
dev-gcc run-with-times.py x11 axpy soa vector quad   1024 200000

# Compute-bound ? 1024 2000000

dev-gcc run-with-times.py x11 axpy aos vector half   1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector float  1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector double 1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector long   1024 2000000

dev-gcc run-with-times.py x11 axpy soa vector half   1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector float  1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector double 1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector long   1024 2000000

# -O3

dev-gcc compile.py g++ c++23 O3 axpy

dev-gcc run-with-times.py x11 axpy aos vector half   1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector float  1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector double 1024 2000000
dev-gcc run-with-times.py x11 axpy aos vector long   1024 2000000

dev-gcc run-with-times.py x11 axpy soa vector half   1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector float  1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector double 1024 2000000
dev-gcc run-with-times.py x11 axpy soa vector long   1024 2000000


