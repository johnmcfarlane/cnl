#!/usr/bin/env bash

mkdir /tmp/cnl
cd /tmp/cnl

apt-get update
apt-get install -y $1 cmake libboost-dev g++-8-arm-linux-gnueabi

export CXX=arm-linux-gnueabi-g++-8
cmake -DCMAKE_BUILD_TYPE=Release -DCNL_INT128=OFF -DCMAKE_CXX_FLAGS="-march=armv7-a" -DCNL_TEST_VC=OFF  /root/project
