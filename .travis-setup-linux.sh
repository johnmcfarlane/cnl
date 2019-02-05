#!/usr/bin/env bash

# Before install
sudo apt-get update -q

# remove GCC-4.x
sudo apt remove gcc

# Install
sudo apt-get install -q cmake ninja-build doxygen cloc
cmake --version

# Ubuntu 14.04 defaults to Boost v54 which doesn't work without RTTI
if [[ "$EXCEPTIONS" == "ON" ]]; then
    sudo apt-get install -q libboost-dev
fi
