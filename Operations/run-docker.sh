#!/usr/bin/env bash
echo RUN gcc:11.2.0 ./run.sh $*
docker run --rm -v "${PWD}":/work -w /work -it gcc:11.2.0 ./run.sh $*
