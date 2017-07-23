cmake_minimum_required(VERSION 3.2)
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # add a target to generate preprocessor output
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
    set(SINGLE_HEADER_OUTPUT cnl_complete.h)
    set(FLAGS "-E" "-I${CMAKE_CURRENT_LIST_DIR}/../../include" "-std=c++11" "-DCNL_DISABLE_GCC_BUILTINS")
    add_custom_target(single_header
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/header.h > ${SINGLE_HEADER_OUTPUT}
            COMMAND ${CMAKE_CXX_COMPILER} ${FLAGS} ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp | grep "^[^#]" | grep --after-context=1000000 single_header_delimiter | tail -n +2 >> ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${SINGLE_HEADER_OUTPUT}
            )
endif ()
