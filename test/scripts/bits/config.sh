#!/usr/bin/env bash

# Configure the build system with given CMake configuration options

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../../..
  pwd
)

cloc "${PROJECT_DIR}"/include
cloc "${PROJECT_DIR}"/test

conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCNL_DEV=ON \
  -G Ninja \
  "$@" \
  "${PROJECT_DIR}"
