#!/usr/bin/env bash

set -e

c++ --version
cmake --version
cloc include
cloc src

cmake \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCNL_DEV=ON -DCNL_STD=${STD} \
  -DCNL_EXCEPTIONS=${EXCEPTIONS} \
  -DCNL_INT128=${INT128} \
  -DCNL_SANITIZE=${SANITIZE} \
  -G "${GENERATOR}"

cmake --build . -- -j 8

export LSAN_OPTIONS=verbosity=1:log_threads=1
ctest --output-on-failure -j 8

./src/benchmark/Benchmark --benchmark_format=csv>result.csv
./src/benchmark/report.py result.csv
ls -l src/test src/benchmark/Benchmark
