cmake_minimum_required(VERSION 2.8.4)
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

######################################################################
# fp_test target

add_executable(fp_test
        ${CMAKE_CURRENT_LIST_DIR}/elastic.cpp
        ${CMAKE_CURRENT_LIST_DIR}/common.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/glm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/index.cpp
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/multiprecision.cpp
        ${CMAKE_CURRENT_LIST_DIR}/p0381.cpp
        ${CMAKE_CURRENT_LIST_DIR}/p0037.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_built_in.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_saturated_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_native_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_throwing_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cstdint.cpp
        ${CMAKE_CURRENT_LIST_DIR}/readme.cpp
        ${CMAKE_CURRENT_LIST_DIR}/snippets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_math.cpp
)

######################################################################
# find external lib, boost.multiprecision

# older versions of CMake don't know how to find multiprecision...
#find_package(Boost COMPONENTS multiprecision)

# ...so the entire library is sought
find_package(Boost)

if(Boost_FOUND)
    # only compile multiprecision.cpp if Boost.Multiprecision is available
    set(BOOST_FOUND_CXX_FLAGS "-DSG14_BOOST_ENABLED")
    target_include_directories(fp_test SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
    target_link_libraries(fp_test ${Boost_LIBRARIES})
endif(Boost_FOUND)

######################################################################
# pull external project, glm

ExternalProject_Add(
    glm
    URL "https://github.com/johnmcfarlane/glm/archive/a4d22127522833607d646ea3622a3a1e329ea67b.zip"
    URL_MD5 "9d866b271b271f84be456e709bc51c6d"
    CMAKE_ARGS GLM_TEST_ENABLE_CXX_11:BOOL=ON -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(glm source_dir)
target_include_directories(
    fp_test
    PRIVATE
    SYSTEM
    ${source_dir}
)

add_dependencies(
    fp_test
    glm
)

######################################################################
# pull external project, google_test

# helpful example [here](https://gist.github.com/Fraser999/5351180)
ExternalProject_Add(
        google_test
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "release-1.7.0"
        INSTALL_COMMAND ""
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
)

ExternalProject_Get_Property(google_test source_dir)
target_include_directories(
        fp_test
        PRIVATE
        SYSTEM
        ${source_dir}/include
)

ExternalProject_Get_Property(google_test binary_dir)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
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

add_dependencies(
        fp_test
        google_test
)

######################################################################
# pull external project, safe_numerics

set(SAFE_NUMERICS OFF CACHE BOOL "compile with support for safe_numerics library")
if(SAFE_NUMERICS AND Boost_FOUND AND NOT APPLE)
    # test conditions necessary to use safe_numerics
    if (NOT CPP14)
        message(FATAL_ERROR "safe_numerics library requires C++14 (CPP14=ON)")
    endif (NOT CPP14)

    if (NOT EXCEPTIONS)
        message(FATAL_ERROR "safe_numerics library requires exception handling (EXCEPTIONS=ON)")
    endif (NOT EXCEPTIONS)

    ExternalProject_Add(
            safe_numerics
            GIT_REPOSITORY "https://github.com/johnmcfarlane/safe_numerics.git"
            GIT_TAG "d28240ebd67bdb0378d983fe09ac87a8f98c5339"
            INSTALL_COMMAND ""
    )

    ExternalProject_Get_Property(safe_numerics SOURCE_DIR)
    target_include_directories(
            fp_test
            PRIVATE
            SYSTEM
            ${SOURCE_DIR}/include
    )

    add_dependencies(
            fp_test
            safe_numerics
    )

    target_compile_definitions(fp_test PRIVATE SG14_SAFE_NUMERICS_ENABLED=1)
endif()

######################################################################
# fp_test compiler flags

set_target_properties(
        fp_test
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${BOOST_FOUND_CXX_FLAGS}"
)

# avoid GCC6 warning
# http://stackoverflow.com/a/38650955/671509
if (${CMAKE_COMPILER_IS_GNUCXX})
    set_source_files_properties(
            ${CMAKE_CURRENT_LIST_DIR}/multiprecision.cpp
            ${CMAKE_CURRENT_LIST_DIR}/index.cpp
            PROPERTIES COMPILE_FLAGS "-fpermissive")
endif ()

######################################################################
# testing

enable_testing()
add_test(all_tests fp_test)
