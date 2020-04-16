#!/usr/bin/env bash

set -ex

SCRIPT_DIR=$(dirname "$0")

STD=${1:-17}
GENERATOR=${2:-"Unix Makefiles"}
NUM_CPUS=${3:-$(nproc)}
PROJECT_SOURCE_DIR=${4:-"${SCRIPT_DIR}/.."}

if [ "$5" == ON ]
then
  CLANG_TIDY="clang-tidy"
fi

cloc "${PROJECT_SOURCE_DIR}"/include
cloc "${PROJECT_SOURCE_DIR}"/test

cc --version
c++ --version
cmake --version
conan --version

conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl

build_and_test () {
  cmake \
    -DCMAKE_BUILD_TYPE="$1" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_CXX_CLANG_TIDY=${CLANG_TIDY} \
    -DCNL_DEV=ON -DCNL_STD=${STD} \
    -DCNL_EXCEPTIONS=$2 \
    -DCNL_INT128=$3 \
    -DCNL_SANITIZE=${SANITIZE} \
    -G "${GENERATOR}" \
    "${PROJECT_SOURCE_DIR}"

  cmake --build . -- -j $NUM_CPUS
  ctest --output-on-failure \
    -j $NUM_CPUS \
    -E Tidy-_impl-wide_integer-literals\|Tidy-static_integer-operators
}

ccache --show-stats
build_and_test Debug OFF OFF
build_and_test Release ON ON
ccache --show-stats

bin/Benchmark --benchmark_format=csv>result.csv
"${PROJECT_SOURCE_DIR}"/test/benchmark/report.py result.csv
ls -l bin
