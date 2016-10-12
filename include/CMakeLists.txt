project(fixed_point)

# CMake 2.8.11 is required by Google Benchmarks
cmake_minimum_required(VERSION 2.8.11)

add_library(
        fixed_point STATIC
        include/sg14/type_traits.h
        include/sg14/auxiliary/multiprecision.h
        include/sg14/auxiliary/elastic_integer.h
        include/sg14/auxiliary/integer.h
        include/sg14/auxiliary/safe_integer.h
        include/sg14/auxiliary/elastic.h
        include/sg14/bits/fixed_point_math.h
        include/sg14/bits/fixed_point_operators.h
        include/sg14/bits/fixed_point_make.h
        include/sg14/bits/fixed_point_arithmetic.h
        include/sg14/bits/fixed_point_type.h
        include/sg14/bits/fixed_point_common_type.h
        include/sg14/bits/fixed_point_named.h
        include/sg14/bits/int128.h
        include/sg14/bits/fixed_point_extras.h
        include/sg14/bits/common.h
        include/sg14/bits/config.h
        include/sg14/fixed_point.h
)
set_target_properties(fixed_point PROPERTIES LINKER_LANGUAGE CXX)
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR})
