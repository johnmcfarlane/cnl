#!/usr/bin/env bash

# Configure, build and run performance benchmarks

set -euo pipefail

NUM_CPUS=${NUM_CPUS:-$(nproc)}
PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/bits/config.sh" \
  -DCNL_INT128=ON \
  "$@"

"${PROJECT_DIR}/test/scripts/bits/build.sh" test-benchmark

test/benchmark/test-benchmark --benchmark_format=csv | tee result.csv
"${PROJECT_DIR}"/test/benchmark/report.py result.csv
