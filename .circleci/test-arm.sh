#!/usr/bin/env bash

set -ex

export CXX=arm-linux-gnueabi-g++-10

apt-get update
apt-get install --quiet --yes g++-10-arm-linux-gnueabi libboost-dev

mkdir /tmp/cnl
cd /tmp/cnl

conan profile update settings.arch=armv7 default
conan profile update settings.compiler.libcxx=libstdc++11 default

conan remote add \
  johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl

/root/project/test/scripts/bits/config.sh \
  -DCNL_INT128=OFF \
  -DCMAKE_TOOLCHAIN_FILE=/root/project/test/cmake/toolchain/gcc-armv7-a.cmake

/root/project/test/scripts/bits/build.sh \
  test-all
