#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake
from conans import tools

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
        "clang_tidy": [False, True],
        "cppcheck": [False, True],
        "enable_exceptions": [False, True],
        "int128": [False, True],
        "iostreams": [False, True],
        "sanitize": [False, True],
        "test": [None, "unit", "benchmark", "all"]
    }
    default_options = {
        "clang_tidy": False,
        "cppcheck": False,
        "enable_exceptions": True,
        "int128": True,
        "iostreams": True,
        "sanitize": False,
        "test": None
    }
    _cmake = None

    generators = "cmake_find_package"
    no_copy_source = True

    def requirements(self):
        fail, msg = self._validate()
        assert not fail, msg

        if self.options.test:
            self.requires("gtest/1.10.0")
            self.requires("benchmark/1.5.2")

    scm = {
        "revision": "auto",
        "type": "git",
        "url": "https://github.com/johnmcfarlane/cnl.git",
    }

    @property
    def _minimum_cpp_standard(self):
        return 20

    @property
    def _minimum_compilers_version(self):
        return {
            "Visual Studio": "16.11",
            "gcc": "10",
            "clang": "11"
        }

    def _validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            tools.check_min_cppstd(self, self._minimum_cpp_standard)
        min_version = self._minimum_compilers_version.get(str(self.settings.compiler))

        def lazy_lt_semver(v1, v2):
            lv1 = [int(v) for v in v1.split(".")]
            lv2 = [int(v) for v in v2.split(".")]
            min_length = min(len(lv1), len(lv2))
            return lv1[:min_length] < lv2[:min_length]

        msg = None
        if not min_version:
            msg = "{} recipe lacks information about {} compiler support.".format(
                self.name, self.settings.compiler)
        else:
            if lazy_lt_semver(str(self.settings.compiler.version), min_version):
               msg = "{0} requires {1} {2}. The current compiler is {1} {3}.".format(self.name,
                                                                                     self.settings.compiler,
                                                                                     min_version,
                                                                                     self.settings.compiler.version)
               return True, msg

        return False, msg

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self, set_cmake_flags=True)
        self._cmake.verbose = False

        self._cmake.definitions["CMAKE_CXX_CLANG_TIDY"] = "clang-tidy" if self.options.clang_tidy else ""
        self._cmake.definitions["CMAKE_CXX_CPPCHECK"] = "cppcheck" if self.options.cppcheck else ""
        self._cmake.definitions["CMAKE_SKIP_INSTALL_ALL_DEPENDENCY"] = "OFF" if self.options.test else "ON"
        self._cmake.definitions["CNL_EXCEPTIONS"] = "ON" if self.options.enable_exceptions else "OFF"
        self._cmake.definitions["CNL_GTEST_MAIN_TARGET:STRING"] = "GTest::gtest_main"
        self._cmake.definitions["CNL_INT128"] = "ON" if self.options.int128 else "OFF"
        self._cmake.definitions["CNL_IOSTREAMS"] = "ON" if self.options.iostreams else "OFF"
        self._cmake.definitions["CNL_SANITIZE"] = "ON" if self.options.sanitize else "OFF"

        self._cmake.configure()
        return self._cmake

    def build(self):

        cmake = self._configure_cmake()

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
        self.copy("LICENSE_1_0.txt", dst="licenses")

    def package_id(self):
        self.info.header_only()

    def package_info(self):
        self.cpp_info.names["cmake_find_package"] = "Cnl"

    def test_phase(self, cmake, test_pattern):
        parallel = "--parallel {}".format(tools.cpu_count()) if cmake.parallel else ""
        self.run("ctest --output-on-failure {} --tests-regex {}".format(
            parallel,
            test_pattern
        ))
