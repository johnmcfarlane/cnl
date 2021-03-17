# root test target
add_custom_target(test-all)

# test customisation flags
set(CNL_EXCEPTIONS ON CACHE BOOL "compile with exceptions enabled")
if (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_ENABLED_FLAGS}")
else (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_DISABLED_FLAGS}")
endif (CNL_EXCEPTIONS)

set(CNL_INT128 ON CACHE BOOL "compile with support for 128-bit integers (where available)")
if (CNL_INT128)
    set(INT128_FLAGS "${INT128_ENABLED_FLAGS}")
else (CNL_INT128)
    set(INT128_FLAGS "${INT128_DISABLED_FLAGS}")
endif (CNL_INT128)

set(CNL_SANITIZE OFF CACHE BOOL "compile with UB sanitizer enabled")
if (CNL_SANITIZE)
    set(SANITIZE_CXX_FLAGS "${SANITIZE_ENABLED_CXX_FLAGS}")
    set(SANITIZE_LINKER_FLAGS "${SANITIZE_ENABLED_LINKER_FLAGS}")
endif (CNL_SANITIZE)

set(COMMON_CXX_FLAGS "${MISC_FLAGS} ${EXCEPTION_FLAGS} ${INT128_FLAGS}")
set(COMMON_LINK_FLAGS Cnl "${CONAN_LIBS}")

# benchmark tests
find_package(benchmark)
if (${benchmark_FOUND})
    add_subdirectory(benchmark)
else ()
    message(STATUS "Google Benchmark is required to build test-benchmark.")
endif ()

# unit tests
find_package(GTest)
if (${GTest_FOUND})
    # workaround for github.com/conan-io/conan-center-index/issues/3222
    set(CNL_GTEST_MAIN_TARGET GTest::Main CACHE STRING "alternative GTest target name")

    add_subdirectory(unit)
else ()
    message(STATUS "Google Test is required to build unit tests.")
endif ()
