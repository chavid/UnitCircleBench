#!/usr/bin/env bash

img=${1}
shift

echo "$img>" $*
docker run --user $(id -u):$(id -g) --rm -v "${UNIT_DIR}":/work -w /work -it $img bin/docker-wrapper.bash $*

if [ $? -ne 0 ]; then
  echo DOCKER FAILED
  exit 1
fi
