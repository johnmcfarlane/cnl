cmake_minimum_required(VERSION 2.8.4)
project(fixed_point)

# build flags
if (EXCEPTIONS)
    set(EXCEPTION_FLAGS "-fexceptions -frtti")
else (EXCEPTIONS)
    set(EXCEPTION_FLAGS "-fno-exceptions -fno-rtti")
endif (EXCEPTIONS)

set(WARNING_FLAGS "-Wall -Wextra -Wfatal-errors -Werror")
set(COMMON_CXX_FLAGS "${WARNING_FLAGS} ${EXCEPTION_FLAGS} -std=c++11")

add_executable(
        run_tests
        src/fixed_point_test.cpp
        src/fixed_point_utils_test.cpp
        src/proposal_test.cpp
        src/run_tests.cpp
        src/test_utils.h
)
set_target_properties(
        run_tests
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}")

add_executable(
        run_benchmarks
        src/benchmark.cpp
        src/sample_functions.h
)
set_target_properties(
        run_benchmarks
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} -fno-omit-frame-pointer")

include_directories("include")
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
target_link_libraries(run_benchmarks benchmark)
