#!/usr/bin/env bash

rm -f tmp.fma.exe && g++ -std=c++17 -O0 -march=native fma.cpp -o tmp.fma.exe && ./tmp.fma.exe
rm -f tmp.fma.exe && g++ -std=c++17 -O2 -march=native fma.cpp -o tmp.fma.exe && ./tmp.fma.exe
rm -f tmp.fma.exe && g++ -std=c++17 -O2 -march=native -ffp-contract=off fma.cpp -o tmp.fma.exe && ./tmp.fma.exe

