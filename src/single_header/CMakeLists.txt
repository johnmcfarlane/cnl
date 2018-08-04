include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

function(make_single_header target output header version macro_flag)
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
        set(output_path "${PROJECT_SOURCE_DIR}/doc/gh-pages/${output}")
    add_custom_target(
            ${target}
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/${header} > ${output_path}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/standard_includes.h >> ${output_path}
            COMMAND ${CMAKE_CXX_COMPILER}
                        "-std=${version}" "${macro_flag}" "-DNDEBUG" "-E" "-I${CMAKE_CURRENT_LIST_DIR}/../../include"
                        ${CMAKE_CURRENT_LIST_DIR}/single_header.cpp
                            | grep "^[^#]"
                            | grep --after-context=1000000 single_header_delimiter
                            | tail -n +2 >> ${output_path}
            COMMAND cat ${CMAKE_CURRENT_LIST_DIR}/footer.h >> ${output_path}
    )
    if (TARGET Doc)
        add_dependencies(Doc ${target})
    endif (TARGET Doc)
endfunction()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    make_single_header(SingleHeader11     cnl_complete_11.h        header11.h "c++11" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader11_128 cnl_complete_11_128bit.h header11.h "c++11" "-DCNL_USE_INT128")
    make_single_header(SingleHeader14     cnl_complete_14.h        header14.h "c++14" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader14_128 cnl_complete_14_128bit.h header14.h "c++14" "-DCNL_USE_INT128")
    make_single_header(SingleHeader17     cnl_complete_17.h        header17.h "c++17" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader17_128 cnl_complete_17_128bit.h header17.h "c++17" "-DCNL_USE_INT128")
endif ()
