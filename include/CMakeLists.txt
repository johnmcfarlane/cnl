project(fixed_point)

# CMake 2.8.11 is required by Google Benchmarks
cmake_minimum_required(VERSION 2.8.11)

add_library(fixed_point INTERFACE)
target_sources(fixed_point INTERFACE
        include/cnl/auxiliary/boost.simd.h
        include/cnl/auxiliary/boost.multiprecision.h
        include/cnl/auxiliary/elastic_integer.h
        include/cnl/auxiliary/elastic_fixed_point.h
        include/cnl/auxiliary/numeric.h
        include/cnl/auxiliary/overflow.h
        include/cnl/auxiliary/safe_integer.h
        include/cnl/bits/fixed_point_math.h
        include/cnl/bits/fixed_point_operators.h
        include/cnl/bits/fixed_point_make.h
        include/cnl/bits/fixed_point_arithmetic.h
        include/cnl/bits/fixed_point_type.h
        include/cnl/bits/fixed_point_common_type.h
        include/cnl/bits/fixed_point_named.h
        include/cnl/bits/fixed_point_extras.h
        include/cnl/bits/common.h
        include/cnl/bits/config.h
        include/cnl/cstdint
        include/cnl/fixed_point
        include/cnl/limits
        include/cnl/type_traits
)

include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR})
install(
        DIRECTORY include/cnl
        DESTINATION include
)
