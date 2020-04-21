#!/usr/bin/env bash

set -ex

STD=$1

mkdir /tmp/cnl
cd /tmp/cnl

apt-get update
apt-get install --quiet --yes ccache clang-tidy cloc cmake libboost-dev python3 python3-pip shellcheck
pip3 install conan

conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default

/root/project/test/scripts/unit-test.sh \
  -DCMAKE_CXX_STANDARD="${STD}" \
  -DCMAKE_TOOLCHAIN_FILE=/root/project/test/cmake/toolchain/gcc.cmake \
  -G "Unix Makefiles"
