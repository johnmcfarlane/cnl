cmake_minimum_required(VERSION 3.2)
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif ()

include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # add a target to generate preprocessor output
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
    set(SINGLE_HEADER_OUTPUT ${CMAKE_CURRENT_LIST_DIR}/fixed_point.h)
    add_custom_target(single_header
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/header.h > ${SINGLE_HEADER_OUTPUT}
            COMMAND ${CMAKE_CXX_COMPILER} ${COMMON_CXX_FLAGS_LIST} -E -I ${CMAKE_CURRENT_LIST_DIR}/../../include ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp | grep "^[^#]" | grep --after-context=1000000 single_header_delimiter | tail +2 - >> ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${SINGLE_HEADER_OUTPUT}
            )
endif ()
