#!/usr/bin/env bash

img=silkeh/clang:19-bookworm
echo "$img>" $*
docker run --rm -v "${PWD}":/work -w /work -it $img $*

if [ $? -ne 0 ]; then
  echo DOCKER FAILED
  exit 1
fi
