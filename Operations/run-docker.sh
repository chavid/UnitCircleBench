#!/usr/bin/env bash
echo RUN gcc:14.2 ./run.sh $*
docker run --rm -v "${PWD}":/work -w /work -it gcc:14.2 ./run.sh $*
