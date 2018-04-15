include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

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

ExternalProject_Add(
  Gbm
  URL "https://github.com/google/benchmark/archive/v1.2.0.tar.gz"
  URL_MD5 "48d0b090cd7a84af2c4a28c8dc963c74"
  CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:STRING=<INSTALL_DIR>
)

ExternalProject_Get_Property(Gbm source_dir)
target_include_directories(
        Benchmark SYSTEM
        PRIVATE ${source_dir}/include
)

# not sure why this isn't taken care of in gbm/src/CMakeLists.txt
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(Benchmark Shlwapi)
endif()

find_package(Threads REQUIRED)

######################################################################
# hitch gbm to test

add_dependencies(Benchmark Gbm)
ExternalProject_Get_Property(Gbm binary_dir)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  target_link_libraries(
    Benchmark
    debug ${binary_dir}/src/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${binary_dir}/src/Release/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
  )
else ()
  target_link_libraries(
    Benchmark
    general ${binary_dir}/src/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark.a
    general pthread
  )
endif ()
