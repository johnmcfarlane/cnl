include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # add a target to generate preprocessor output
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
    set(FLAGS "-E" "-I${CMAKE_CURRENT_LIST_DIR}/../../include" "-DCNL_DISABLE_GCC_BUILTINS")

    set(SINGLE_HEADER_OUTPUT cnl_complete_11.h)
    add_custom_target(single_header_11
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/header11.h > ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/standard_includes.h >> ${SINGLE_HEADER_OUTPUT}
            COMMAND ${CMAKE_CXX_COMPILER} "-std=c++11" ${FLAGS} ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp | grep "^[^#]" | grep --after-context=1000000 single_header_delimiter | tail -n +2 >> ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${SINGLE_HEADER_OUTPUT}
            )

    set(SINGLE_HEADER_OUTPUT cnl_complete_14.h)
    add_custom_target(single_header_14
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/header14.h > ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/standard_includes.h >> ${SINGLE_HEADER_OUTPUT}
            COMMAND ${CMAKE_CXX_COMPILER} "-std=c++14" ${FLAGS} ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp | grep "^[^#]" | grep --after-context=1000000 single_header_delimiter | tail -n +2 >> ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${SINGLE_HEADER_OUTPUT}
            )

    set(SINGLE_HEADER_OUTPUT cnl_complete_17.h)
    add_custom_target(single_header_17
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/header17.h > ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/standard_includes.h >> ${SINGLE_HEADER_OUTPUT}
            COMMAND ${CMAKE_CXX_COMPILER} "-std=c++17" ${FLAGS} ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp | grep "^[^#]" | grep --after-context=1000000 single_header_delimiter | tail -n +2 >> ${SINGLE_HEADER_OUTPUT}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${SINGLE_HEADER_OUTPUT}
            )
endif ()
