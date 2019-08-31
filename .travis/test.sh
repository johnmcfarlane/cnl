#!/usr/bin/env bash

set -ex

c++ --version
cmake --version
cloc "${PROJECT_SOURCE_DIR}"/include
cloc "${PROJECT_SOURCE_DIR}"/src

export LSAN_OPTIONS=verbosity=1:log_threads=1

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

ccache -s
build_and_test Debug OFF OFF
build_and_test Release ON ON
ccache -s

src/benchmark/Benchmark --benchmark_format=csv>result.csv
"${PROJECT_SOURCE_DIR}"/src/benchmark/report.py result.csv
ls -l src/test src/benchmark/Benchmark
