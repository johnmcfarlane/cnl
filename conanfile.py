#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile
from conans.tools import load
import re, os

class CnlConan(ConanFile):
    name = "cnl"
    def set_version(self):
        content = load(os.path.join(self.recipe_folder, "CMakeLists.txt"))
        version = re.search(r"project\(cnl VERSION (.*)\)", content).group(1)
        self.version = version.strip()
    license = "Boost Software License 1.0"
    author = "John McFarlane <cnl@john.mcfarlane.name>"
    url = "https://github.com/johnmcfarlane/cnl"
    homepage = "https://johnmcfarlane.github.io/cnl/"
    description = "A Compositional Numeric Library for C++"
    topics = ("fixed-point", "value-types")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_paths"
    no_copy_source = True
    requires = "gtest/1.10.0","benchmark/1.5.0@johnmcfarlane/stable"

    scm = {
        "type": "git",
        "url": "https://github.com/johnmcfarlane/cnl.git",
        "revision": "auto"
    }

    def package(self):
        self.copy("include/*.h")
        self.copy("LICENSE_1_0.txt", "licenses")

    def package_id(self):
        self.info.header_only()
