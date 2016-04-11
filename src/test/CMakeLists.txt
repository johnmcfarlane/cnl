cmake_minimum_required(VERSION 2.8.4)
include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

######################################################################
# list fp_test source files

set(    SOURCES
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

######################################################################
# find external lib, boost.multiprecision

# older versions of CMake don't know how to find multiprecision...
#find_package(Boost COMPONENTS multiprecision)

# ...so the entire library is sought
find_package(Boost)

if(Boost_FOUND)
    # only compile multiprecision.cpp if Boost.Multiprecision is available
    set(SOURCES ${SOURCES} ${CMAKE_CURRENT_LIST_DIR}/multiprecision.cpp)
endif(Boost_FOUND)

######################################################################
# fp_test target

add_executable(fp_test ${SOURCES})

set_target_properties(
        fp_test
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
        OUTPUT_NAME "fp_test"
)

# add external lib, Boost
if(Boost_FOUND)
    target_include_directories(fp_test PRIVATE ${Boost_INCLUDE_DIR})
    target_link_libraries(fp_test ${Boost_LIBRARIES})
endif(Boost_FOUND)

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
target_include_directories(
        fp_test
        PRIVATE
        SYSTEM
        ${source_dir}/include
)

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
