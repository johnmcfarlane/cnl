#!/usr/bin/env bash

set -e

ccache -s

# from https://github.com/i3/i3/blob/42f5a6ce479968a8f95dd5a827524865094d6a5c/.travis.yml
docker login -u ${DOCKER_USER} -p ${DOCKER_PASS}
docker run \
  --env BUILD_TYPE="${BUILD_TYPE}" \
  --env EXCEPTIONS="${EXCEPTIONS}" \
  --env GENERATOR="${GENERATOR}" \
  --env INT128="${INT128}" \
  --env SANITIZE="${SANITIZE}" \
  --env STD="${STD}" \
  -v $PWD:/ws/ \
  -v ~/.ccache/:/root/.ccache/ \
  -w /ws \
  johnmcfarlane/cnl_ci:${IMAGE} ./.travis/test.sh

ccache -s
