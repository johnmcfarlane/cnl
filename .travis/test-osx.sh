#!/usr/bin/env bash

set -e

# Before install
brew update

# Install
brew install ccache cloc ${PACKAGE}
brew upgrade python

# Advice from docs.travis-ci.com/user/caching/#ccache-cache
export PATH="/usr/local/opt/ccache/libexec:$PATH"

CC=${C_COMPILER}
CXX=${CXX_COMPILER}

.travis/test.sh
