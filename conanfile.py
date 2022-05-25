#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake
from conans import tools

required_conan_version = ">=1.43.0"

class CnlConan(ConanFile):
    name = "cnl"
    license = "Boost Software License 1.0"
    author = "John McFarlane <cnl@john.mcfarlane.name>"
    url = "https://github.com/johnmcfarlane/cnl"
    homepage = "https://johnmcfarlane.github.io/cnl/"
    description = "A Compositional Numeric Library for C++"
    topics = ("fixed-point", "value-types")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "cppcheck": [False, True],
        "enable_exceptions": [False, True],
        "int128": [None, False, True],
        "iostreams": [False, True],
        "sanitize": [False, True],
        "test": [None, "unit", "benchmark", "all"]
    }
    default_options = {
        "cppcheck": False,
        "enable_exceptions": True,
        "int128": None,
        "iostreams": True,
        "sanitize": False,
        "test": None,
    }
    generators = "cmake_find_package"
    no_copy_source = True
    scm = {
        "revision": "auto",
        "type": "git",
        "url": "https://github.com/johnmcfarlane/cnl.git",
    }

    def build_requirements(self):
        self.build_requires("cmake/[>=3.15]")

        if self.options.test in ("benchmark", "all"):
            self.test_requires("benchmark/[>=1.6]")

        if self.options.test in ("unit", "all"):
            self.test_requires("boost/1.77.0")
            self.test_requires("gtest/cci.20210126")

    def build(self):
        cmake = CMake(self, set_cmake_flags=True)
        cmake.verbose = False

        cmake.definitions["CMAKE_CXX_CPPCHECK"] = "cppcheck" if self.options.cppcheck else ""
        cmake.definitions["CMAKE_SKIP_INSTALL_ALL_DEPENDENCY"] = "OFF" if self.options.test else "ON"
        cmake.definitions["CNL_EXCEPTIONS"] = "ON" if self.options.enable_exceptions else "OFF"
        cmake.definitions["CNL_GTEST_MAIN_TARGET:STRING"] = "GTest::gtest_main"
        if (str(self.options.int128) != "None"):
            cmake.definitions["CNL_INT128"] = "ON" if self.options.int128 else "OFF"
        cmake.definitions["CNL_IOSTREAMS"] = "ON" if self.options.iostreams else "OFF"
        cmake.definitions["CNL_SANITIZE"] = "ON" if self.options.sanitize else "OFF"

        cmake.configure()

        if self.should_build and self.options.test:
            cmake.build(target={
              "unit": "test-unit",
              "benchmark": "test-benchmark",
              "all": "test-all"}[str(self.options.test)])

        if self.should_test and self.options.test:
            self.test_phase(cmake, {
              "unit": "^test-unit-",
              "benchmark": "test-benchmark",
              # "^test-unit-" here instead of "^test-" because benchmarks are of little value
              # if their results are not output, so they are typically run separate from ctest
              "all": "^test-unit-"}[str(self.options.test)])

        if self.should_install:
            cmake.install()

    def package(self):
        self.copy("include/*.h")
        self.copy("LICENSE_1_0.txt", "licenses")

    def package_id(self):
        self.info.header_only()

    def test_phase(self, cmake, test_pattern):
        parallel = "--parallel {}".format(tools.cpu_count()) if cmake.parallel else ""
        self.run("ctest --output-on-failure {} --tests-regex {}".format(
            parallel,
            test_pattern
        ))
