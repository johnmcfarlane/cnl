include("../../cmake/common.cmake")

######################################################################
# Benchmark target

add_executable(
        Benchmark
        ${CMAKE_CURRENT_LIST_DIR}/benchmark.cpp
)

set_target_properties(
        Benchmark
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
)

target_link_libraries(Benchmark Cnl)

######################################################################
# add external project, google/benchmark

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(Benchmark Shlwapi)
endif()

find_package(Threads REQUIRED)

######################################################################
# hitch gbm to test

target_link_libraries(Benchmark ${CONAN_LIBS})
if (IS_CLANG_FAMILY OR IS_GCC_FAMILY)
  target_link_libraries(Benchmark pthread)
endif ()
