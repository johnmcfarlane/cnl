#!/usr/bin/env bash

set -euo pipefail

EXCEPTIONS=${EXCEPTIONS:-ON}
GENERATOR=${GENERATOR:-Ninja}
INT128=${INT128:-ON}
STANDARD=${STANDARD:-17}
TOOLCHAIN=${TOOLCHAIN:-gcc}

CONTAINER_PROJECT_DIR=/cnl
PROJECT_DIR=$(
  cd "$(dirname "$0")"/..
  pwd
)

ccache --show-stats

docker run \
  --cap-add SYS_PTRACE \
  --volume ${PROJECT_DIR}:"${CONTAINER_PROJECT_DIR}" \
  --volume "${HOME}"/.ccache:/root/.ccache \
  --volume "${HOME}"/.conan:/root/.conan \
  --workdir /ws \
  "johnmcfarlane/${IMG}" \
  bash -c "\"${CONTAINER_PROJECT_DIR}/test/scripts/${SCRIPT}.sh\" \
    -DCMAKE_CXX_STANDARD=\"${STANDARD}\" \
    -DCMAKE_TOOLCHAIN_FILE=\"${CONTAINER_PROJECT_DIR}/test/cmake/toolchain/${TOOLCHAIN}\".cmake \
    -DCNL_EXCEPTIONS=\"${EXCEPTIONS}\" \
    -DCNL_INT128=\"${INT128}\" \
    -G \"${GENERATOR}\""

ccache --show-stats
