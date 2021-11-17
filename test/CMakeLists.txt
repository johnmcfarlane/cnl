# root test target
add_custom_target(test-all)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CNL_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CNL_CXX_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CNL_EXE_LINKER_FLAGS}")

# test customisation flags
set(CNL_EXCEPTIONS ON CACHE BOOL "compile with exceptions enabled")
if (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_ENABLED_FLAGS}")
else (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_DISABLED_FLAGS}")
endif (CNL_EXCEPTIONS)

set(CNL_INT128 "" CACHE BOOL "compile with support for 128-bit integers (where available)")
if (NOT "${CNL_INT128}" STREQUAL "")
    if (CNL_INT128)
        set(INT128_FLAGS "${INT128_ENABLED_FLAGS}")
    else (CNL_INT128)
        set(INT128_FLAGS "${INT128_DISABLED_FLAGS}")
    endif (CNL_INT128)
endif ()

set(CNL_IOSTREAMS ON CACHE BOOL "compile with support for IO streams")
if (CNL_IOSTREAMS)
    set(IOSTREAMS_FLAGS "-DCNL_USE_IOSTREAMS=1")
else (CNL_IOSTREAMS)
    set(IOSTREAMS_FLAGS "-DCNL_USE_IOSTREAMS=0")
endif (CNL_IOSTREAMS)

set(CNL_SANITIZE OFF CACHE BOOL "compile with UB sanitizer enabled")
if (CNL_SANITIZE)
    set(SANITIZE_CXX_FLAGS "${SANITIZE_ENABLED_CXX_FLAGS}")
    set(SANITIZE_LINKER_FLAGS "${SANITIZE_ENABLED_LINKER_FLAGS}")
endif (CNL_SANITIZE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXCEPTION_FLAGS} ${INT128_FLAGS} ${IOSTREAMS_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZE_LINKER_FLAGS}")

# benchmark tests
find_package(benchmark QUIET)
if (${benchmark_FOUND})
    add_subdirectory(benchmark)
else ()
    message(STATUS "Google Benchmark is required to build test-benchmark.")
endif ()

# unit tests
find_package(GTest QUIET)
if (${GTest_FOUND})
    # workaround for github.com/conan-io/conan-center-index/issues/3222
    set(CNL_GTEST_MAIN_TARGET GTest::Main CACHE STRING "alternative GTest target name")

    add_subdirectory(unit)
else ()
    message(STATUS "Google Test is required to build unit tests.")
endif ()
