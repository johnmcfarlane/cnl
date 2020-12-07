#!/usr/bin/env bash

# Configure, build and run all tests

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

"${PROJECT_DIR}"/test/scripts/test.sh \
  --options target=test-all
