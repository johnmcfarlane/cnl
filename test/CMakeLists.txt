include(cmake/common.cmake)

include(ExternalProject)

add_custom_target(test-all)

find_package(benchmark)
if (${benchmark_FOUND})
    add_subdirectory(benchmark)
else ()
    message(STATUS "Google Benchmark is required to build test-benchmark.")
endif ()

find_package(GTest)
if (${GTest_FOUND})
    add_subdirectory(unit)
else ()
    message(STATUS "Google Test is required to build unit tests.")
endif ()
