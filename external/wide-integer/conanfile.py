#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import CMake, ConanFile, tools

class WideIntegerConan(ConanFile):
    name = "wide-integer"
    license = "Boost Software License 1.0"
    author = "Christopher Kormanyos <ckormanyos@yahoo.com>"
    url = "https://github.com/johnmcfarlane/wide-integer"
    homepage = "https://github.com/ckormanyos/wide-integer"
    description = "Wide-Integer implements a generic C++ template for uint128_t, uint256_t, uint512_t, uint1024_t, etc."
    topics = ("high-performance", "embedded-systems", "multiprecision", "numerical", "big-integer")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"

    scm = {
        "revision": "149ec8d70e029a104472df01cc1895b06154da7d",
        "type": "git",
        "url": "https://github.com/johnmcfarlane/wide-integer.git",
    }

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        cmake.test(output_on_failure=True, target="all")

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_id(self):
        self.info.header_only()
