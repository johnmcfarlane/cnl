#!/usr/bin/env bash

# Generate documentation in doc/gh-pages directory

set -euo pipefail

BITS_DIR=$(
  cd "$(dirname "$0")"/bits
  pwd
)

"${BITS_DIR}/config.sh" \
  "$@"

"${BITS_DIR}/build.sh" Doc
