#!/usr/bin/env bash

set -ex

# Advice from docs.travis-ci.com/user/caching/#ccache-cache
PATH="/usr/local/opt/ccache/libexec:$PATH"

NUM_CPUS=4
PROJECT_SOURCE_DIR="${TRAVIS_BUILD_DIR}"

# Before install
brew update

# Install
brew install ccache cloc conan ${PACKAGE}

conan profile new default --detect
if [ "$CXX" != "clang++" ]
then
  conan profile update settings.compiler.libcxx=libstdc++11 default
fi

.travis/test.sh ${STD} "${GENERATOR}" ${NUM_CPUS} "${PROJECT_SOURCE_DIR}" ON
