#!/usr/bin/env bash

set -ex

export CXX=arm-linux-gnueabi-g++-9

apt-get update
apt-get install --quiet --yes g++-9-arm-linux-gnueabi libboost-dev

conan profile update settings.arch=armv7 default
conan profile update settings.compiler.libcxx=libstdc++11 default

mkdir /tmp/cnl
cd /tmp/cnl

/root/project/test/scripts/bits/config.sh \
  -DCNL_DEV=ON \
  -DCNL_INT128=OFF \
  -DCMAKE_TOOLCHAIN_FILE=/root/project/test/cmake/toolchain/gcc-armv7-a.cmake

/root/project/test/scripts/bits/build.sh \
  test-all
