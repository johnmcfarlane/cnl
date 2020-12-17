#!/usr/bin/env bash

# Configure and build all targets with Clang-Tidy static analysis,
# assuming a Clang toolchain

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/test.sh" \
  --env CONAN_CMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}/test/cmake/toolchain/clang-tidy.cmake" \
  --env CXX=clang++
