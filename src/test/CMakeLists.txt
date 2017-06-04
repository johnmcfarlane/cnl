cmake_minimum_required(VERSION 2.8.4)
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

######################################################################
# fp_test target

add_executable(fp_test
        # slowest
        ${CMAKE_CURRENT_LIST_DIR}/elastic_fixed_point.cpp

        # still pretty slow
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_built_in.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_saturated_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_native_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_throwing_integer.cpp

        ${CMAKE_CURRENT_LIST_DIR}/overflow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/safe_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precise_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/const_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/common.cpp
        ${CMAKE_CURRENT_LIST_DIR}/elastic_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/glm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/index.cpp
        ${CMAKE_CURRENT_LIST_DIR}/make_elastic_fixed_point.cpp
        ${CMAKE_CURRENT_LIST_DIR}/multiprecision.cpp
        ${CMAKE_CURRENT_LIST_DIR}/number_test.cpp
        ${CMAKE_CURRENT_LIST_DIR}/numeric_traits.cpp
        ${CMAKE_CURRENT_LIST_DIR}/p0037.cpp
        ${CMAKE_CURRENT_LIST_DIR}/p0381.cpp
        ${CMAKE_CURRENT_LIST_DIR}/p0554.cpp
        ${CMAKE_CURRENT_LIST_DIR}/readme.cpp
        ${CMAKE_CURRENT_LIST_DIR}/snippets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/fixed_point_math.cpp
        ${CMAKE_CURRENT_LIST_DIR}/zero_cost_average.cpp
        ${CMAKE_CURRENT_LIST_DIR}/zero_cost_square.cpp
        ${CMAKE_CURRENT_LIST_DIR}/safft.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cppnow2017.cpp

        # likely to fail if other files with simpler tests fail
        ${CMAKE_CURRENT_LIST_DIR}/safe_elastic_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precise_elastic_integer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precise_fixed_point.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precise_safe_elastic_fixed_point.cpp
        ${CMAKE_CURRENT_LIST_DIR}/precise_safe_elastic_integer.cpp

        # pointless unless the rest compiles
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

######################################################################
# find external lib, boost.multiprecision

# older versions of CMake don't know how to find multiprecision...
#find_package(Boost COMPONENTS multiprecision)

# ...so the entire library is sought
find_package(Boost)

if(Boost_FOUND)
    if(BOOST_VERSION GREATER 105500 OR EXCEPTIONS)
        # only compile multiprecision.cpp if Boost.Multiprecision is available
        set(BOOST_FOUND_CXX_FLAGS "-DSG14_BOOST_ENABLED")
        target_include_directories(fp_test SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
        target_link_libraries(fp_test ${Boost_LIBRARIES})
    else(BOOST_VERSION GREATER 105500 OR EXCEPTIONS)
        message("Boost version is too low to use without exception handling")
    endif(BOOST_VERSION GREATER 105500 OR EXCEPTIONS)
else(Boost_FOUND)
    message("Boost not found")
endif(Boost_FOUND)

######################################################################
# pull external project, glm

ExternalProject_Add(
    glm
    URL "https://github.com/johnmcfarlane/glm/archive/878086621522255b14017c26d1df3b9fb0b36009.zip"
    URL_MD5 "fe3b032c230df63474bc26549526aaec"
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
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

ExternalProject_Add(
        google_test
        URL "https://github.com/google/googletest/archive/c99458533a9b4c743ed51537e25989ea55944908.zip"
        URL_MD5 "4552721bde3dcaab1eaa9582afc28c9d"
        UPDATE_COMMAND ""
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
