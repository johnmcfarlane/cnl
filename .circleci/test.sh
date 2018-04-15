#!/usr/bin/env bash

mkdir /tmp/cnl
cd /tmp/cnl

apt-get update
apt-get install -y $1 cmake libboost-dev

cmake -DINT128=$2 -DSTD=$3 /root/project
cmake --build . -- -j 8

ctest
src/benchmark/fp_benchmark
