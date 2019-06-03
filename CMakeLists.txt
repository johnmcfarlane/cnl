project(cnl)

cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

set(CNL_DEV OFF CACHE BOOL "configure build for running tests and benchmarks")

if (CNL_DEV)
    # runs a suite of compile-time tests using `static_assert`
    # and run-time tests using gtest
    include(CTest)
    add_subdirectory("src/test")

    # performs a selection of benchmark tests using googletest
    add_subdirectory("src/benchmark")

    # generate documentation
    add_subdirectory("doc")

    # generate single-header header
    add_subdirectory("src/single_header")
endif (CNL_DEV)

# the CNL library
add_library(Cnl INTERFACE)

target_include_directories(
        Cnl INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS Cnl EXPORT CnlTargets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT CnlTargets
        FILE CnlConfig.cmake
        NAMESPACE Cnl::
        DESTINATION lib/cmake/cnl)
