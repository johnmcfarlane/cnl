#!/usr/bin/env bash

set -euo pipefail

EXCEPTIONS=${EXCEPTIONS:-ON}
GENERATOR=${GENERATOR:-"Unix Makefiles"}
INT128=${INT128:-ON}
STANDARD=${STANDARD:-17}

# Advice from docs.travis-ci.com/user/caching/#ccache-cache
PATH="/usr/local/opt/ccache/libexec:$PATH"

export NUM_CPUS=4
PROJECT_DIR="${TRAVIS_BUILD_DIR}"

# Before install
brew update
brew unlink python@2

# Install
brew install ccache cloc conan
if [ -n "${PACKAGE-}" ]; then
  brew install "${PACKAGE}"
fi

conan profile new default --detect
if [ "$CXX" != "clang++" ]; then
  conan profile update settings.compiler=gcc default
  conan profile update settings.compiler.libcxx=libstdc++11 default
  conan profile update settings.compiler.version="${VERSION}" default
fi

pwd
mkdir -p ../build && cd ../build

"${PROJECT_DIR}/test/scripts/${SCRIPT}.sh" \
  -DCMAKE_CXX_STANDARD="${STANDARD}" \
  -DCNL_EXCEPTIONS="${EXCEPTIONS}" \
  -DCNL_INT128="${INT128}" \
  -DCNL_SANITIZE=OFF \
  -G "${GENERATOR}"
