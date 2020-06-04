#!/usr/bin/env bash

# Configure the build system with given CMake configuration options

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../../..
  pwd
)

cloc "${PROJECT_DIR}"/include
cloc "${PROJECT_DIR}"/test

conan install \
  --build=missing \
  "${PROJECT_DIR}"

cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_PROJECT_INCLUDE:FILEPATH="$(pwd)"/conan_paths.cmake \
  -G Ninja \
  "$@" \
  "${PROJECT_DIR}"
