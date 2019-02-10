include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

function(make_single_header target output header version int128_flag gcc_intrinsics_flag)
    string(REPLACE " " ";" COMMON_CXX_FLAGS_LIST ${COMMON_CXX_FLAGS})
    set(output_path "${PROJECT_SOURCE_DIR}/doc/gh-pages/${output}")
    set(standard_includes_dir "${CMAKE_CURRENT_BINARY_DIR}")
    set(standard_includes_path "${standard_includes_dir}/standard_includes.h")
    add_custom_target(
            ${target}
            ALL
            cat ${CMAKE_CURRENT_LIST_DIR}/${header} > ${output_path}
            COMMAND grep -rh \"\#include <\" ${PROJECT_SOURCE_DIR}/include/ | sort | uniq | grep -v boost >> "${standard_includes_path}"
            COMMAND cat "${standard_includes_path}" >> ${output_path}
            COMMAND ${CMAKE_CXX_COMPILER}
                        "-std=${version}" "-DCNL_USE_INT128=${int128_flag}" "${gcc_intrinsics_flag}" "-DNDEBUG" "-E" "-I${CMAKE_CURRENT_LIST_DIR}/../../include"
                        "-I${standard_includes_dir}"
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
    make_single_header(SingleHeader11     cnl_complete_11.h        header11.h "c++11" "0" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader11_128 cnl_complete_11_128bit.h header11.h "c++11" "1" "-DCNL_USE_GCC_INTRINSICS=1")
    make_single_header(SingleHeader14     cnl_complete_14.h        header14.h "c++14" "0" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader14_128 cnl_complete_14_128bit.h header14.h "c++14" "1" "-DCNL_USE_GCC_INTRINSICS=1")
    make_single_header(SingleHeader17     cnl_complete_17.h        header17.h "c++17" "0" "-DCNL_USE_GCC_INTRINSICS=0")
    make_single_header(SingleHeader17_128 cnl_complete_17_128bit.h header17.h "c++17" "1" "-DCNL_USE_GCC_INTRINSICS=1")
endif ()
