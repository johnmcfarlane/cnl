#!/usr/bin/env bash

set -ex

GENERATOR="Unix Makefiles"
NUM_CPUS=8
PROJECT_SOURCE_DIR=/root/project
SANITIZE=ON
STD=$1

mkdir /tmp/cnl
cd /tmp/cnl

apt-get update
apt-get install --quiet --yes ccache clang-tidy cloc cmake libboost-dev python3 python3-pip
pip3 install conan

conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default

/root/project/.travis/test.sh ${STD} "${GENERATOR}" ${NUM_CPUS} "${PROJECT_SOURCE_DIR}" ON
