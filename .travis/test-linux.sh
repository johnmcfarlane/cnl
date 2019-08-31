#!/usr/bin/env bash

set -e

# from https://github.com/i3/i3/blob/42f5a6ce479968a8f95dd5a827524865094d6a5c/.travis.yml
docker login -u ${DOCKER_USER} -p ${DOCKER_PASS}
docker run \
  --env GENERATOR="${GENERATOR}" \
  --env NUM_CPUS=$(nproc) \
  --env PROJECT_SOURCE_DIR=/cnl \
  --env SANITIZE="${SANITIZE}" \
  --env STD="${STD}" \
  -v "${TRAVIS_BUILD_DIR}":/cnl/ \
  -v ~/.ccache/:/root/.ccache/ \
  -w /ws \
  johnmcfarlane/cnl_ci:${IMAGE} /cnl/.travis/test.sh
