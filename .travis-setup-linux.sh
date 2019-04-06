#!/usr/bin/env bash

# Before install
sudo apt-get update -q

# remove GCC-4.x
sudo apt remove gcc

# Install
sudo apt-get install -q cmake ninja-build doxygen cloc libboost-dev
cmake --version
