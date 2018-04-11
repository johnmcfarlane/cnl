project(cnl)

cmake_minimum_required(VERSION 3.2)
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

# runs a suite of compile-time tests using `static_assert`
# and run-time tests using gtest
include(CTest)
add_subdirectory("src/test")

# performs a selection of benchmark tests using googletest
add_subdirectory("src/benchmark")

# generate single-header header
add_subdirectory("src/single_header")

# generate documentation
add_subdirectory("doc")

# the CNL library
add_library(cnl INTERFACE)

set(cnl_public_headers
        ${PROJECT_SOURCE_DIR}/include/cnl/integral_constant.h
        ${PROJECT_SOURCE_DIR}/include/cnl/auxiliary/boost.multiprecision.h
        ${PROJECT_SOURCE_DIR}/include/cnl/auxiliary/boost.simd.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bit.h
        ${PROJECT_SOURCE_DIR}/include/cnl/cnlint.h
        ${PROJECT_SOURCE_DIR}/include/cnl/constant.h
        ${PROJECT_SOURCE_DIR}/include/cnl/elastic_integer.h
        ${PROJECT_SOURCE_DIR}/include/cnl/elastic_number.h
        ${PROJECT_SOURCE_DIR}/include/cnl/fixed_point.h
        ${PROJECT_SOURCE_DIR}/include/cnl/fractional.h
        ${PROJECT_SOURCE_DIR}/include/cnl/limits.h
        ${PROJECT_SOURCE_DIR}/include/cnl/num_traits.h
        ${PROJECT_SOURCE_DIR}/include/cnl/numeric.h
        ${PROJECT_SOURCE_DIR}/include/cnl/overflow.h
        ${PROJECT_SOURCE_DIR}/include/cnl/overflow_integer.h
        ${PROJECT_SOURCE_DIR}/include/cnl/rounding_integer.h
        )

set(cnl_implementation_headers
        ${PROJECT_SOURCE_DIR}/include/cnl/auxiliary/boost.multiprecision.h
        ${PROJECT_SOURCE_DIR}/include/cnl/auxiliary/boost.simd.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/config.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/encompasses.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/operators.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/math.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/type.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/common_type.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/extras.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/named.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/num_traits.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/fixed_point/constants.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/type_traits.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/operators.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/common.h
        ${PROJECT_SOURCE_DIR}/include/cnl/bits/number_base.h
        )

target_sources(cnl INTERFACE ${cnl_public_headers} ${cnl_implementation_headers})

target_include_directories(cnl INTERFACE include)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/cnl
        DESTINATION include)

configure_file("${PROJECT_SOURCE_DIR}/cmake/CnlConfig.cmake.in"
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CnlConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/CnlConfig.cmake"
        DESTINATION lib/cmake/cnl)
