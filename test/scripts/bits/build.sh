#!/usr/bin/env bash

# Build the given target

set -euo pipefail

NUM_CPUS=${NUM_CPUS:-$(nproc)}

ccache --show-stats

cmake \
  --build . \
  --target "$1" \
  -- -j "${NUM_CPUS}"

ccache --show-stats
