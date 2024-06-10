#!/usr/bin/env bash
docker run --rm -v "${PWD}":/work -w /work -it gcc:11.2.0 ./compile.sh $*
