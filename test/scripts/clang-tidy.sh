#!/usr/bin/env bash

# Configure and build all targets with Clang-Tidy static analysis,
# assuming a Clang toolchain

set -exuo pipefail

export CXX=clang++
PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/bits/config.sh" \
  -DCMAKE_CXX_CLANG_TIDY=clang-tidy \
  -DCMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}/test/cmake/toolchain/clang.cmake" \
  -DCNL_INT128=ON \
  "$@" \
  "${PROJECT_DIR}"

"${PROJECT_DIR}/test/scripts/bits/build.sh" test-all
