#!/usr/bin/env bash

set -e

NUM_CPUS=$(nproc)
PROJECT_SOURCE_DIR=/cnl

# from https://github.com/i3/i3/blob/42f5a6ce479968a8f95dd5a827524865094d6a5c/.travis.yml
docker login -u ${DOCKER_USER} -p ${DOCKER_PASS}
docker run \
  -v "${TRAVIS_BUILD_DIR}":/cnl/ \
  -v ~/.ccache/:/root/.ccache/ \
  -w /ws \
  johnmcfarlane/cnl_ci:${IMAGE} \
  /cnl/.travis/test.sh \
    "${CLANG_TIDY}" \
    "${GENERATOR}" \
    "${NUM_CPUS}" \
    "${PROJECT_SOURCE_DIR}" \
    "${SANITIZE}" \
    "${STD}"
