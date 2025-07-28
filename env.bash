
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

cpp20 () { ${UNIT_BIN}/run-with-docker.bash chavid/devcpp20:v14.2 $* ; }
clang19 () { ${UNIT_BIN}/run-with-docker.bash silkeh/clang:19-bookworm $* ; }
kwk () { ${UNIT_BIN}/run-with-docker.bash chavid/kiwaku:v14.2 $* ; }
export -f cpp20 clang19 kwk

