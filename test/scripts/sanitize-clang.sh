#!/usr/bin/env bash

# Configure, build and run all tests with UBSan and MSan enabled, assuming a Clang toolchain

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/test.sh" \
  --env CONAN_CMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}/test/cmake/toolchain/clang.cmake" \
  --env CXX=clang++ \
  --env LSAN_OPTIONS=verbosity=1:log_threads=1 \
  --options int128=True \
  --options sanitize=True
