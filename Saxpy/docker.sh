#!/usr/bin/env bash
img=gcc:14.2
echo "$img>" $*
docker run --rm -v "${PWD}":/work -w /work -it $img $*
