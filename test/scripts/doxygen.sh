#!/usr/bin/env bash

# Generate documentation in doc/gh-pages directory

set -euo pipefail

BUILD_DIR=$(pwd)
PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCNL_DEV=ON \
  -G Ninja \
  "${PROJECT_DIR}"

cmake \
  --build "${BUILD_DIR}" \
  --target Doc
