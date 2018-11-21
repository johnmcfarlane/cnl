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

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  set(gbm_byproducts
          ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
          ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
          )
  set(gbm_libs
          debug ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
          optimized ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark${CMAKE_FIND_LIBRARY_SUFFIXES}
          )
else ()
  set(gbm_byproducts
          ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark.a
          )
  set(gbm_libs
          ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}benchmark.a
          pthread
          )
endif ()

ExternalProject_Add(
  Gbm
  URL "https://github.com/google/benchmark/archive/v1.2.0.tar.gz"
  URL_MD5 "48d0b090cd7a84af2c4a28c8dc963c74"
  CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_BYPRODUCTS ${gbm_byproducts}
)

ExternalProject_Get_Property(Gbm source_dir)
target_include_directories(
        Benchmark SYSTEM
        PRIVATE ${source_dir}/include
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(Benchmark Shlwapi)
endif()

find_package(Threads REQUIRED)

######################################################################
# hitch gbm to test

add_dependencies(Benchmark Gbm)
target_link_libraries(Benchmark ${gbm_libs})
