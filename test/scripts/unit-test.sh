#!/usr/bin/env bash

# Configure, build and run all unit tests

set -euo pipefail

NUM_CPUS=${NUM_CPUS:-$(nproc)}
PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}/test/scripts/bits/config.sh" \
  -DCMAKE_MODULE_PATH="$(pwd)" \
  "$@"

"${PROJECT_DIR}/test/scripts/bits/build.sh" test-all

ctest \
  --output-on-failure \
  --parallel "${NUM_CPUS}" \
  --tests-regex ^test-unit-
