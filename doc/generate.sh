#!/usr/bin/env bash

# Generate documentation in ./gh-pages

set -euo pipefail

# Dependencies missing from the CI image.
pip3 install sphinx breathe

# CNL observes a strict separation of project from build directory.
PROJECT_DIR=$(
  cd "$(dirname "$0")"/..
  pwd
)

# Sphinx does not appear to support separation of project from build
# so copy essential project files into build directory.
mkdir -p source
cp "${PROJECT_DIR}/doc/conf.py" source

# From source code, use Doxygen to generate XML files.
wget -N http://upload.cppreference.com/mwiki/images/f/f8/cppreference-doxygen-web.tag.xml
CNL_DIR="${PROJECT_DIR}" doxygen "${PROJECT_DIR}/doc/Doxyfile"

# From XML, use Breathe to generate ReST files.
breathe-apidoc \
  --generate class,interface,struct,union,file,namespace,group \
  --output-dir source \
  xml

# From ReST files, use Sphinx to generate HTML files.
sphinx-build source gh-pages
