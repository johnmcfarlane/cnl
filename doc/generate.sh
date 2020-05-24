#!/usr/bin/env bash

# Generate documentation in ./gh-pages

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/..
  pwd
)

wget -N http://upload.cppreference.com/mwiki/images/f/f8/cppreference-doxygen-web.tag.xml
CNL_DIR="${PROJECT_DIR}" doxygen "${PROJECT_DIR}/doc/Doxyfile"
