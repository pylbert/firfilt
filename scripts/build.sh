#!/bin/bash

# Fail on first error
set -e

# Default to a Release build
BUILD_TYPE=Release
# Default to empty for LINUX
SWIG_ROOT=
GTEST_ROOT=
NODE_ROOT=
JAVA_HOME=

# Use git to determine top-level directory
ROOT=$(git rev-parse --show-toplevel)

cmd_gen="cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B${ROOT}/build -H${ROOT}"
cmd_build="cmake --build ${ROOT}/build --config ${BUILD_TYPE}"
cmd_test="ctest -C Release"

# Print out target commands for reference
echo
echo ================================================================================
echo === Running the following cmake commands =======================================
echo ================================================================================
echo BUILD_TYPE=${BUILD_TYPE}
echo SWIG_ROOT=${SWIG_ROOT}
echo GTEST_ROOT=${GTEST_ROOT}
echo NODE_ROOT=${NODE_ROOT}
echo JAVA_HOME=${JAVA_HOME}
echo ROOT=${ROOT}
echo ${cmd_gen}
echo ${cmd_build}
echo pushd build
echo ${cmd_test}
echo popd
echo ================================================================================
echo

# Run the target commands
${cmd_gen}
${cmd_build}
pushd build
${cmd_test}
popd
