add_executable(test-benchmark benchmark.cpp)

set_target_properties(
        test-benchmark
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
)

target_link_libraries(test-benchmark Cnl ${CONAN_LIBS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(test-benchmark Shlwapi)
elseif(IS_CLANG_FAMILY OR IS_GCC_FAMILY)
  find_package(Threads REQUIRED)
  target_link_libraries(test-benchmark pthread)
endif()

