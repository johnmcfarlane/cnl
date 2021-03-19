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
    options = {"enable_exceptions": [False, True],
               "int128": [False, True],
               "sanitize": [False, True],
               "target": ["test-all", "test-benchmark", "test-unit"]}
    default_options = {"enable_exceptions": True,
                       "int128": True,
                       "sanitize": False,
                       "target": "test-unit"}
    generators = "cmake_find_package"
    no_copy_source = True
    requires = "gtest/1.10.0", "benchmark/1.5.2"

    scm = {
        "revision": "auto",
        "type": "git",
        "url": "https://github.com/johnmcfarlane/cnl.git",
    }

    def build(self):
        cmake = CMake(self, set_cmake_flags=True)

        if self.should_configure:
            self.configure_phase(cmake)

        if self.should_build:
            self.build_phase(cmake)

        cmake.install()

        if self.should_test:
            self.test_phase();

    def package(self):
        self.copy("include/*.h")
        self.copy("LICENSE_1_0.txt", "licenses")

    def package_id(self):
        self.info.header_only()

    def configure_phase(self, cmake):
        def conan_option_to_cmake_boolean(conan_option: bool):
            return {
                True: "ON",
                False: "OFF"
            }[bool(conan_option)]

        std = " -DCMAKE_CXX_STANDARD={}".format(
            self.settings.compiler.cppstd) if self.settings.compiler.cppstd else ""
        module_path = "-DCMAKE_MODULE_PATH:FILEPATH={}".format(
            self.build_folder)
        gtest_hack = "-DCNL_GTEST_MAIN_TARGET:STRING=GTest::gtest_main"
        exceptions = "-DCNL_EXCEPTIONS={}".format(
            conan_option_to_cmake_boolean(self.options.enable_exceptions))
        int128 = "-DCNL_INT128={}".format(
            conan_option_to_cmake_boolean(self.options.int128))
        sanitize = "-DCNL_SANITIZE={}".format(
            conan_option_to_cmake_boolean(self.options.sanitize))

        self.run(f'cmake {cmake.command_line}{std} {module_path} {gtest_hack} {exceptions} {int128} {sanitize} {self.source_folder}')
    
    def build_phase(self, cmake):
        build = "--build {}".format(self.build_folder)
        target = "--target {}".format(self.options.target)

        self.run(f'cmake {build} {target} {cmake.build_config}')
    
    def test_phase(self):
        target_to_test_pattern = {
            "test-all": "^test-",
            "test-benchmark": "test-benchmark",
            "test-unit": "^test-unit-"
        }

        self.run("ctest --output-on-failure --parallel {} --tests-regex {}".format(
            tools.cpu_count(),
            target_to_test_pattern[str(self.options.target)]
        ))
