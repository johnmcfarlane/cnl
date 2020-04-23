#!/usr/bin/env bash

set -e

NUM_CPUS=$(nproc)
PROJECT_SOURCE_DIR=/cnl

docker run \
  -v "${TRAVIS_BUILD_DIR}":/cnl/ \
  -v ~/.ccache/:/root/.ccache/ \
  -w /ws \
  johnmcfarlane/cnl_ci:"${IMAGE}" \
  /cnl/.travis/test.sh "${STD}" "${GENERATOR}" "${NUM_CPUS}" "${PROJECT_SOURCE_DIR}" "${CLANG_TIDY}"
