#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile


class CnlConan(ConanFile):
    name = "cnl"
    license = "Boost Software License 1.0"
    author = "John McFarlane <cnl@john.mcfarlane.name>"
    url = "https://github.com/johnmcfarlane/cnl"
    homepage = "https://johnmcfarlane.github.io/cnl/"
    description = "A Compositional Numeric Library for C++"
    topics = ("fixed-point", "value-types")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    no_copy_source = True


    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }

    def package(self):
        self.copy("*.h")
        self.copy("LICENSE_1_0.txt", "licenses")

    def package_id(self):
        self.info.header_only()