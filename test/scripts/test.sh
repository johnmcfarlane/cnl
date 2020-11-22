#!/usr/bin/env bash

# Configure, build and run tests with given `conan install` parameters

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

conan install \
  --build missing \
  --env CONAN_CMAKE_GENERATOR=Ninja \
  "${PROJECT_DIR}" \
  "$@"

conan build \
  --build \
  --configure \
  --test \
  "${PROJECT_DIR}"
