cmake_minimum_required(VERSION 2.8.4)
project(fixed_point)

set(WARNING_FLAGS "-Wall -Wextra -Wfatal-errors -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -fno-rtti -Ofast -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "-g")

add_executable(
        run_tests
        src/fixed_point_test.cpp
        src/fixed_point_utils_test.cpp
        src/proposal_test.cpp
        src/run_tests.cpp
        src/test_utils.h
)

add_executable(
        run_benchmarks
        src/benchmark.cpp
        src/sample_functions.h
)

include_directories("include")
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
target_link_libraries(run_benchmarks benchmark)
