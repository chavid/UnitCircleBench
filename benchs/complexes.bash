#!/usr/bin/env bash

## O0, 1024*10000
#
#dev-icpx compile.py icpx c++23 O0 complexes
#
#dev-icpx run-with-times.py x11 complexes aos vector half   1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector float  1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector double 1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector long   1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector quad   1024 10000
#
#dev-icpx run-with-times.py x11 complexes soa vector half   1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector float  1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector double 1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector long   1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector quad   1024 10000

## O2, 1024*10000
#
#dev-icpx compile.py icpx c++23 O2 complexes
#
#dev-icpx run-with-times.py x11 complexes aos vector float  1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector double 1024 10000
#dev-icpx run-with-times.py x11 complexes aos vector long   1024 10000
#
#dev-icpx run-with-times.py x11 complexes soa vector float  1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector double 1024 10000
#dev-icpx run-with-times.py x11 complexes soa vector long   1024 10000

## O2, 1024*100000

dev-icpx compile.py icpx c++23 O2 complexes

#dev-icpx run-with-times.py x11 complexes aos vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector long   1024 100000
#
#dev-icpx run-with-times.py x11 complexes soa vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector long   1024 100000

## O3, 1024*100000
#
#dev-icpx compile.py icpx c++23 O3 complexes
#
#dev-icpx run-with-times.py x11 complexes aos vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector long   1024 100000
#
#dev-icpx run-with-times.py x11 complexes soa vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector long   1024 100000

## Ofast, 1024*100000
#
#dev-icpx compile.py icpx c++23 Ofast complexes
#
#dev-icpx run-with-times.py x11 complexes aos vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes aos vector long   1024 100000
#
#dev-icpx run-with-times.py x11 complexes soa vector float  1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector double 1024 100000
#dev-icpx run-with-times.py x11 complexes soa vector long   1024 100000


