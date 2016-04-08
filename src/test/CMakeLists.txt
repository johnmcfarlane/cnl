cmake_minimum_required(VERSION 2.8.4)
include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

######################################################################
# fp_test target

add_executable(
        fp_test
        ${CMAKE_CURRENT_LIST_DIR}/elastic1.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic3.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic4.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic5.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic6.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic7.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic8.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/proposal.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_built_in.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_saturated_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_native_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_throwing_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/type_traits.cpp
        ${CMAKE_CURRENT_LIST_DIR}/readme.cpp
        ${CMAKE_CURRENT_LIST_DIR}/snippets.cpp
)
set_target_properties(
        fp_test
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
        OUTPUT_NAME "fp_test"
)

target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/assert/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/array/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/concept_check/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/config/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/container/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/core/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/detail/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/format/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/iterator/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/integer/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/lexical_cast/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/math/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/move/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/mpl/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/multiprecision/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/numeric_conversion/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/optional/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/predef/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/preprocessor/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/range/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/rational/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/smart_ptr/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/static_assert/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/throw_exception/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/type_traits/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/ublas/include)
target_include_directories(fp_test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../boost/utility/include)

######################################################################
# add external project, google_test

# helpful example [here](https://gist.github.com/Fraser999/5351180)
ExternalProject_Add(
        google_test
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "release-1.7.0"
        INSTALL_COMMAND ""
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
)
ExternalProject_Get_Property(google_test source_dir)
include_directories(SYSTEM ${source_dir}/include)

######################################################################
# hitch google_test to fp_test

add_dependencies(fp_test google_test)
ExternalProject_Get_Property(google_test binary_dir)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_link_libraries(
            fp_test
            debug ${binary_dir}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${binary_dir}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
else ()
    target_link_libraries(
            fp_test
            general ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a
            general pthread
    )
endif ()
