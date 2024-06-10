#!/usr/bin/env bash
docker run --rm -v "${PWD}":/work -w /work -it gcc:10.2.0 ./objdump.sh $*
