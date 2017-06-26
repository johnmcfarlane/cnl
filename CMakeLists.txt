project(fixed_point)

# CMake 2.8.11 is required by Google Benchmarks
cmake_minimum_required(VERSION 2.8.11)

include("include/CMakeLists.txt")

# runs a suite of compile-time tests using `static_assert`
# and run-time tests using gtest
include("src/test/CMakeLists.txt")

# performs a selection of benchmark tests using googletest
include("src/benchmark/CMakeLists.txt")

# generate documentation
include("doc/CMakeLists.txt")

# generate single-header header
include("src/single_header/CMakeLists.txt")
