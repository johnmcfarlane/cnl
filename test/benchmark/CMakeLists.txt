if(CMAKE_CXX_CLANG_TIDY)
    set_property(SOURCE benchmark.cpp APPEND PROPERTY
            OBJECT_DEPENDS "${PROJECT_SOURCE_DIR}/.clang-tidy")
endif()

add_executable(test-benchmark benchmark.cpp)
target_link_libraries(test-benchmark benchmark::benchmark Cnl)

add_dependencies(test-all test-benchmark)
add_test(test-benchmark "${CMAKE_CURRENT_BINARY_DIR}/test-benchmark")
