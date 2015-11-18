project(fixed_point)
cmake_minimum_required(VERSION 2.8.4)

# runs a suite of compile-time tests using `static_assert`
# and run-time tests using gtest
include("src/test/CMakeLists.txt")

# performs a selection of benchmark tests using googletest
include("src/benchmark/CMakeLists.txt")
