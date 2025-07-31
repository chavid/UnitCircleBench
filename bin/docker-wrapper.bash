#!/usr/bin/env bash

if [ -f /mydevtools/bashrc ]; then
    source /mydevtools/bashrc
fi

source env.bash

mkdir -p build

$*

