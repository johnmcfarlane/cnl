#!/usr/bin/env bash

set -ex

STD=$1
GENERATOR=$2
NUM_CPUS=$3
PROJECT_SOURCE_DIR=$4

cloc "${PROJECT_SOURCE_DIR}"/include
cloc "${PROJECT_SOURCE_DIR}"/src

cc --version
c++ --version
cmake --version
conan --version

conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl
conan install --build=outdated "${PROJECT_SOURCE_DIR}"

build_and_test () {
  cmake \
    -DCMAKE_BUILD_TYPE="$1" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCNL_DEV=ON -DCNL_STD=${STD} \
    -DCNL_EXCEPTIONS=$2 \
    -DCNL_INT128=$3 \
    -DCNL_SANITIZE=${SANITIZE} \
    -G "${GENERATOR}" \
    "${PROJECT_SOURCE_DIR}"

  cmake --build . -- -j $NUM_CPUS
  ctest --output-on-failure -j $NUM_CPUS
}

ccache --show-stats
build_and_test Debug OFF OFF
build_and_test Release ON ON
ccache --show-stats

src/benchmark/Benchmark --benchmark_format=csv>result.csv
"${PROJECT_SOURCE_DIR}"/src/benchmark/report.py result.csv
ls -l src/test src/benchmark/Benchmark
