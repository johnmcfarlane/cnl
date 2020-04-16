if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project(cnl VERSION 0.0.1)

cmake_minimum_required(VERSION 3.5.1)

set(CNL_DEV OFF CACHE BOOL "configure build for running tests and benchmarks")

if (CNL_DEV)
    include(cmake/install-conan.cmake)

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
else ()
    message(STATUS "Use CNL_DEV=ON to build tests.")
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
