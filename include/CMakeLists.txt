add_library(cnl INTERFACE)
target_sources(cnl INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.multiprecision.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/boost.simd.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/const_integer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/elastic_fixed_point.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/elastic_integer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/multiprecision.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/numeric.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/overflow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/precise_integer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/auxiliary/safe_integer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/fixed_point.h
        ${CMAKE_CURRENT_SOURCE_DIR}/cnl/num_traits.h
)

include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR})
install(
        DIRECTORY include/cnl
        DESTINATION include
)
