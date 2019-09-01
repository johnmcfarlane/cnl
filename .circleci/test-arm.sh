#!/usr/bin/env bash

set -ex

export CXX=arm-linux-gnueabi-g++-8

apt-get update
apt-get install --quiet --yes $1 ccache cloc cmake libboost-dev g++-8-arm-linux-gnueabi python3 python3-pip
pip3 install conan

conan remote add johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl
conan profile new default
cat ~/.conan/profiles/default
conan profile update settings.arch=armv7 default
conan profile update settings.compiler=gcc default
conan profile update settings.compiler.libcxx=libstdc++11 default
conan profile update settings.compiler.version=8 default
conan profile update settings.os=Linux default

mkdir /tmp/cnl
cd /tmp/cnl

conan install --build=outdated /root/project
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-march=armv7-a" \
    -DCNL_DEV=ON -DCNL_INT128=OFF \
    -DCNL_TEST_VC=OFF \
    /root/project
cmake --build . --target Tests -- -j 8
