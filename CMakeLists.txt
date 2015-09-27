cmake_minimum_required(VERSION 2.8.4)
project(fixed_point_test)
project(fixed_point_benchmark)

option(RUN_TESTS "Run tests against fixed_point." ON)
option(RUN_BENCHMARKS "Generate benchmarks againt fixed_point." OFF)

set(WARNING_FLAGS "-Wall -Wextra -Wfatal-errors -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -fno-rtti -Ofast") # -fno-omit-frame-pointer
set(CMAKE_EXE_LINKER_FLAGS "-g")

set(SOURCE_FILES
    src/main.cpp
    src/fixed_point_test.cpp
    src/fixed_point_utils_test.cpp
    src/proposal_test.cpp
    src/benchmark.cpp)

include_directories("include")

if (RUN_BENCHMARKS)
    add_executable(fixed_point_benchmark ${SOURCE_FILES})

    add_subdirectory(benchmark EXCLUDE_FROM_ALL)
    include_directories("benchmark/include")
    target_link_libraries(fixed_point_benchmark benchmark)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRUN_BENCHMARKS")
endif (RUN_BENCHMARKS)

if (RUN_TESTS)
    add_executable(fixed_point_test ${SOURCE_FILES})

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRUN_TESTS")
    target_link_libraries(fixed_point_test)
endif(RUN_TESTS)
