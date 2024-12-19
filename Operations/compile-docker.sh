#!/usr/bin/env bash
docker run --rm -v "${PWD}":/work -w /work -it gcc:14.2 ./compile.sh $*
