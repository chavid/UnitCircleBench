#!/usr/bin/env bash

# O0, 1024*10000

./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes aos vector half   1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes aos vector float  1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes aos vector double 1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes aos vector long   1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes aos vector quad   1024 10000

./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes soa vector half   1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes soa vector float  1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes soa vector double 1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes soa vector long   1024 10000
./docker-devcpp20.sh  ./time.py c++23 O0 x10 complexes soa vector quad   1024 10000

# O2, 1024*10000

./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector float  1024 10000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector double 1024 10000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector long   1024 10000

./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector float  1024 10000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector double 1024 10000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector long   1024 10000

# O2, 1024*100000

./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes aos vector long   1024 100000

./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 O2 x10 complexes soa vector long   1024 100000

# O3, 1024*100000

./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes aos vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes aos vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes aos vector long   1024 100000

./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes soa vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes soa vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 O3 x10 complexes soa vector long   1024 100000

# Ofast, 1024*100000

./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes aos vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes aos vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes aos vector long   1024 100000

./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes soa vector float  1024 100000
./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes soa vector double 1024 100000
./docker-devcpp20.sh  ./time.py c++23 Ofast x10 complexes soa vector long   1024 100000


