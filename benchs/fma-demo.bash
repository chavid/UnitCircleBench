#!/usr/bin/env bash

rm -f ${UNIT_BUILD}/fma.exe && g++ -std=c++17 -O0 -march=native ${UNIT_SRC}/fma.cpp -o ${UNIT_BUILD}/fma.exe && ${UNIT_BUILD}/fma.exe
rm -f ${UNIT_BUILD}/fma.exe && g++ -std=c++17 -O2 -march=native ${UNIT_SRC}/fma.cpp -o ${UNIT_BUILD}/fma.exe && ${UNIT_BUILD}/fma.exe
rm -f ${UNIT_BUILD}/fma.exe && g++ -std=c++17 -O2 -march=native -ffp-contract=off ${UNIT_SRC}/fma.cpp -o ${UNIT_BUILD}/fma.exe && ${UNIT_BUILD}/fma.exe

