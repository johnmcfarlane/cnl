#!/usr/bin/env bash

# Auto-format C++ code in repository

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

find "${PROJECT_DIR}/include" "${PROJECT_DIR}/test" \( -name "*.cpp" -or -name "*.h" \) -exec clang-format -i {} +
