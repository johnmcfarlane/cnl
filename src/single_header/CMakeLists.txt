include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

function(make_single_header target output header footer flags)
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
        set(output_path "${PROJECT_SOURCE_DIR}/doc/gh-pages/${output}")
    add_custom_target(
            ${target}
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/${header} > ${output_path}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/standard_includes.h >> ${output_path}
            COMMAND ${CMAKE_CXX_COMPILER}
                        "${flags}" "-E" "-DCNL_USE_INT128" "-I${CMAKE_CURRENT_LIST_DIR}/../../include"
                        ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp
                            | grep "^[^#]"
                            | grep --after-context=1000000 single_header_delimiter
                            | tail -n +2 >> ${output_path}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/${footer} >> ${output_path}
    )
endfunction()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    make_single_header(SingleHeader11 cnl_complete_11.h header11.h footer.h "-std=c++11")
    make_single_header(SingleHeader14 cnl_complete_14.h header14.h footer.h "-std=c++14")
    make_single_header(SingleHeader17 cnl_complete_17.h header17.h footer.h "-std=c++17")
endif ()
