#!/usr/bin/env bash

# Before install
brew update

# Install
brew install ccache cloc ${PACKAGE}

# Advice from docs.travis-ci.com/user/caching/#ccache-cache
export PATH="/usr/local/opt/ccache/libexec:$PATH"
