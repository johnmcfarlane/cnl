#!/usr/bin/env bash

# Configure, build and run ShellCheck shell script linter over all scripts in project directory

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/../..
  pwd
)

find "${PROJECT_DIR}" -name "*.sh" | while read -r TEST; do
  shellcheck \
    --check-sourced \
    --color=always \
    --external-sources \
    --severity=info \
    --shell=bash \
    "$TEST"
done

echo shellcheck success
