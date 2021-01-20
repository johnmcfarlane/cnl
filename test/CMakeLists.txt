include(cmake/common.cmake)

add_custom_target(test-all)

find_package(benchmark)
if (${benchmark_FOUND})
    add_subdirectory(benchmark)
else ()
    message(STATUS "Google Benchmark is required to build test-benchmark.")
endif ()

find_package(GTest)
if (${GTest_FOUND})
    # workaround for github.com/conan-io/conan-center-index/issues/3222
    set(CNL_GTEST_MAIN_TARGET GTest::Main CACHE STRING "alternative GTest target name")

    add_subdirectory(unit)
else ()
    message(STATUS "Google Test is required to build unit tests.")
endif ()
