#!/usr/bin/env bash

img=chavid/devcpp20:v14.2
echo "$img>" $*
docker run --rm -v "${PWD}":/work -w /work -it $img $*

if [ $? -ne 0 ]; then
  echo DOCKER FAILED
  exit 1
fi
