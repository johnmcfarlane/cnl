#!/usr/bin/env bash

set -ex

export CXX=arm-linux-gnueabi-g++-8

apt-get update
apt-get install --quiet --yes ccache cloc cmake g++-8-arm-linux-gnueabi libboost-dev ninja-build python3 python3-pip
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

/root/project/test/scripts/bits/config.sh \
  -DCNL_DEV=ON \
  -DCNL_INT128=OFF \
  -DCNL_TEST_VC=OFF \
  -DCMAKE_TOOLCHAIN_FILE=/root/project/test/cmake/toolchain/gcc-armv7-a.cmake

/root/project/test/scripts/bits/build.sh \
  test-all
