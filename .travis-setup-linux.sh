#!/usr/bin/env bash

# Before install
sudo add-apt-repository -y ppa:kalakris/cmake
sudo apt-get update -qq

# remove GCC-4.x
sudo apt remove gcc

# Install
sudo apt-get install -qq cmake
cmake --version

# Ubuntu 14.04 defaults to Boost v54 which doesn't work without RTTI
if [[ "$EXCEPTIONS" == "ON" ]]; then
    sudo apt-get install -qq libboost-dev
fi
