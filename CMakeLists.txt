project(cnl)

cmake_minimum_required(VERSION 3.2)
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

add_subdirectory("include")

# runs a suite of compile-time tests using `static_assert`
# and run-time tests using gtest
include(CTest)
add_subdirectory("src/test")

# performs a selection of benchmark tests using googletest
add_subdirectory("src/benchmark")

# generate single-header header
add_subdirectory("src/single_header")

# generate documentation
add_subdirectory("doc")
