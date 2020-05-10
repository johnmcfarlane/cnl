#!/usr/bin/env bash

# Configure, build and run all tests with UBSan and MSan enabled, assuming a GCC toolchain

set -euo pipefail

export CXX=g++
PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/test-all.sh" \
  -DCMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}/test/cmake/toolchain/gcc.cmake" \
  -DCNL_INT128=ON \
  -DCNL_SANITIZE=ON \
  "$@"
