add_library(cnl INTERFACE)

set(cnl_public_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/integral_constant.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.multiprecision.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.simd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/cnlint.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/constant.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/elastic_integer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/fixed_point.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/limits.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/num_traits.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/numeric.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/overflow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/overflow_int.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/precise_integer.h
)

set(cnl_implementation_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.multiprecision.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.simd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/config.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/encompasses.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/operators.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/math.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/type.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/arithmetic.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/common_type.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/extras.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/named.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/num_traits.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/fixed_point/constants.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/type_traits.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/operators.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/number_base.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/bits/elastic_fixed_point.h
)

target_sources(cnl INTERFACE ${cnl_public_headers} ${cnl_implementation_headers})

target_include_directories(cnl INTERFACE ${CMAKE_CURRENT_LIST_DIR})

install(
        DIRECTORY include/cnl
        DESTINATION include
)
