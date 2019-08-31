#!/usr/bin/env bash

set -ex

# Before install
brew update

# Install
brew install ccache cloc ${PACKAGE}
brew upgrade python

# Advice from docs.travis-ci.com/user/caching/#ccache-cache
export PATH="/usr/local/opt/ccache/libexec:$PATH"

export NUM_CPUS=4
export PROJECT_SOURCE_DIR="${TRAVIS_BUILD_DIR}"

.travis/test.sh
