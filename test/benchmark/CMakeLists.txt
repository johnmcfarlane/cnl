add_executable(test-benchmark benchmark.cpp)

set_target_properties(
        test-benchmark
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
)

target_link_libraries(test-benchmark ${COMMON_LINK_FLAGS})

add_dependencies(test-all test-benchmark)
add_test(test-benchmark "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-benchmark")
