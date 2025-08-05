
ORIGINAL_DIR=${PWD}

# establish the script directory absolute path

THIS_FILE=${BASH_SOURCE[0]}
THIS_DIR=`dirname ${THIS_FILE}`
cd ${THIS_DIR}
UNIT_DIR=`pwd`
cd ${ORIGINAL_DIR}

# update PATH

export UNIT_DIR
export UNIT_BIN=${UNIT_DIR}/bin
export UNIT_SRC=${UNIT_DIR}/src
export UNIT_BUILD=${UNIT_DIR}/build

if [[ ":${PATH}:" != *":${UNIT_BIN}:"* ]]; then
    export PATH="${UNIT_BIN}:${PATH}"
fi

# ensure the presence of build

mkdir -p ${UNIT_BUILD}

# exportable aliases

dev-gcc () { ${UNIT_BIN}/run-with-docker.bash chavid/dev-gcc:15.1.0-bookworm $* ; }
dev-clang () { ${UNIT_BIN}/run-with-docker.bash chavid/dev-clang:20-bookworm $* ; }
dev-icpx () { ${UNIT_BIN}/run-with-docker-gpu.bash chavid/dev-icpx:2025.2-bookworm $* ; }
eve-gcc () { ${UNIT_BIN}/run-with-docker.bash chavid/eve-gcc:15.1.0-bookworm $* ; }
kwk-gcc () { ${UNIT_BIN}/run-with-docker.bash chavid/kiwaku-gcc:15.1.0-bookworm $* ; }
kwk-clang () { ${UNIT_BIN}/run-with-docker.bash chavid/kiwaku-clang:20-bookworm $* ; }
kwk-icpx () { ${UNIT_BIN}/run-with-docker-gpu.bash chavid/kiwaku-icpx:2025.2-bookworm $* ; }
export -f dev-gcc dev-clang dev-icpx eve-gcc kwk-gcc kwk-clang kwk-icpx

