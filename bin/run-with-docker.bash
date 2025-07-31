#!/usr/bin/env bash

img=${1}
shift

echo "$img>" $*
docker run --gpus all --device=/dev/dri --user $(id -u):$(id -g) --rm -v "${UNIT_DIR}":/work -w /work -it -e DTAG=${img} ${img} bin/docker-wrapper.bash $*

if [ $? -ne 0 ]; then
  echo DOCKER FAILED
  exit 1
fi
