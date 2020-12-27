#!/usr/bin/env bash

# installs conan dependencies in working directory;
# to be invoked prior to CMake configuration step

set -euo pipefail

conan remote add \
  johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl

conan profile new \
  --detect \
  default

conan profile update \
  settings.compiler.libcxx=libstdc++11 \
  default
