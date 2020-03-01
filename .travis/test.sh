#!/usr/bin/env bash

set -ex

SCRIPT_DIR=$(dirname "$0")

CLANG_TIDY=${1:-1}
GENERATOR=${2:-"Unix Makefiles"}
NUM_CPUS=${3:-$(nproc)}
PROJECT_SOURCE_DIR=${4:-"${SCRIPT_DIR}/.."}
SANITIZE=${5:-"OFF"}
STD=${6:-17}

cloc "${PROJECT_SOURCE_DIR}"/include
cloc "${PROJECT_SOURCE_DIR}"/src

cc --version
c++ --version
cmake --version
conan --version

conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl

build_and_test () {
  BUILD_TYPE="$1"
  EXCEPTIONS="$2"
  INT128="$3"
  cmake \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCNL_CLANG_TIDY=${CLANG_TIDY} \
    -DCNL_DEV=ON -DCNL_STD=${STD} \
    -DCNL_EXCEPTIONS="${EXCEPTIONS}" \
    -DCNL_INT128="${INT128}" \
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
"${PROJECT_SOURCE_DIR}"/src/benchmark/report.py result.csv
ls -l bin
