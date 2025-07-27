#!/usr/bin/env bash

prog=${1}
shift

objdump -i tmp.${prog}.exe

