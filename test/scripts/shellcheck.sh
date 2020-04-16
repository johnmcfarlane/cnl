#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

find "${PROJECT_DIR}" -name "*.sh" | while read -r TEST; do
  shellcheck \
    --color=always \
    --external-sources \
    --shell=bash \
    "$TEST"
done;

echo shellcheck success
