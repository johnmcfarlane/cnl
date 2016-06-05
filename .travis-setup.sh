#!/usr/bin/env bash

# Before install
sudo add-apt-repository -y ppa:kalakris/cmake
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq

# Install
sudo apt-get install -qq cmake
