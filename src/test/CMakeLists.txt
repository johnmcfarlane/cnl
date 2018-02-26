include("../common/common.cmake")

enable_testing()

# source files containing tests of the correctness of CNL;
# ideally compiles with every warning that CNL user might choose
set(test_sources
        # low-level
        bit.cpp
        cnlint.cpp
        encompasses.cpp
        common.cpp
        limits.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        readme.cpp
        overflow/overflow.cpp

        # components
        constant.cpp
        fixed_point/fixed_point_built_in.cpp
        fixed_point/constants.cpp
        elastic/elastic_integer.cpp
        fixed_point/extras.cpp
        overflow/overflow_integer.cpp
        fixed_point/math.cpp
        rounding/rounding_integer.cpp

        # composites
        fixed_point/overflow/fixed_point_native_integer.cpp
        fixed_point/overflow/fixed_point_throwing_integer.cpp
        fixed_point/overflow/fixed_point_saturated_integer.cpp
        rounding/elastic/rounding_elastic_integer.cpp
        fixed_point/rounding_fixed_point.cpp
        elastic/rounding/overflow/rounding_safe_integer.cpp
        fixed_point/elastic/rounding/overflow/rounding_safe_fixed_point.cpp
        overflow/elastic/safe_integer.cpp
        fixed_point/elastic/make_elastic_fixed_point.cpp
        fixed_point/elastic/elastic_fixed_point.cpp
)

# source files containing demos and examples of CNL;
# do not necessarily need to fail the strictest warnings
set(sample_sources
        # showing off
        index.cpp
        papers/p0381.cpp
        papers/p0675.cpp
        papers/p0827.cpp
        papers/p0828.cpp
        papers/p0554.cpp
        fixed_point/p0037.cpp
        zero_cost_square.cpp
        zero_cost_free_functions.cpp
        snippets.cpp
        zero_cost_average.cpp
        cppnow2017.cpp
        cppcon2017.cpp
        boost.multiprecision.cpp
        glm.cpp
        safft.cpp
)

######################################################################
# pull in external dependencies

find_package(Boost)

ExternalProject_Add(
        google_test
        URL "https://github.com/google/googletest/archive/3a4cf1a02ef4adc28fccb7eef2b573b14cd59009.zip"
        URL_MD5 "06ac495303fbe94b198026e3c196425e"
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
)

ExternalProject_Add(
        glm
        URL "https://github.com/johnmcfarlane/glm/archive/878086621522255b14017c26d1df3b9fb0b36009.zip"
        URL_MD5 "fe3b032c230df63474bc26549526aaec"
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
)

######################################################################
# add_gtest_dependency

function(add_gtest_dependency target)
    # gtest
    ExternalProject_Get_Property(google_test binary_dir)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_link_libraries(
                "${target}"
                debug ${binary_dir}/googlemock/gtest/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtestd${CMAKE_FIND_LIBRARY_SUFFIXES}
                optimized ${binary_dir}/googlemock/gtest/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    else ()
        target_link_libraries(
                "${target}"
                general "${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a"
                general pthread
        )
    endif ()

    ExternalProject_Get_Property(google_test source_dir)
    target_include_directories("${target}" SYSTEM PRIVATE "${source_dir}/googletest/include")

    add_dependencies("${target}" google_test)
endfunction(add_gtest_dependency)

######################################################################
# test main

add_library(test_main main.cpp)
add_gtest_dependency(test_main)

######################################################################
# make_test

# given the path to a source file containing gtest tests,
# creates and returns a target that runs them
function(make_test source out_target compile_flags)
    # turn source file path into target name (e.g. "foo/bar.cpp" -> "test-foo-bar")
    string(REPLACE "\.cpp" "" stripped "${source}")
    string(REPLACE "/" "-" target "test-${stripped}")
    set(${out_target} "${target}" PARENT_SCOPE)

    # create a target and make it a test
    add_executable("${target}" "${source}")
    add_test("${target}" "${target}")
    set_target_properties("${target}" PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${compile_flags}")
    target_link_libraries("${target}" cnl test_main)

    add_gtest_dependency("${target}")

    # Boost
    if(Boost_FOUND)
        if(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
            # only compile multiprecision.cpp if Boost.Multiprecision is available
            target_compile_definitions("${target}" PRIVATE "-DCNL_BOOST_ENABLED")
            target_include_directories("${target}" SYSTEM PRIVATE "${Boost_INCLUDE_DIRS}")
        else(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
            message("Boost version is too low to use without exception handling")
        endif(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
    else(Boost_FOUND)
        message("Boost not found")
    endif(Boost_FOUND)
endfunction(make_test)

######################################################################
# create tests to verify CNL

foreach(source ${test_sources})
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        make_test("${source}" target "")
    elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        make_test("${source}" target "-Wconversion -Wno-sign-conversion")
    endif ()
endforeach(source)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set_source_files_properties(
            cppcon2017.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-strict-overflow")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set_source_files_properties(
            fixed_point/math.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-integer-overflow")
endif ()

######################################################################
# create tests to show off CNL

foreach(source ${sample_sources})
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        make_test("${source}" target "-fpermissive -Wno-sign-compare")
    else ()
        make_test("${source}" target "")
    endif ()
endforeach(source)

######################################################################
# create test of glm integration

ExternalProject_Get_Property(glm source_dir)
target_include_directories(test-glm SYSTEM PRIVATE "${source_dir}")
add_dependencies(test-glm glm)

######################################################################
# create test of boost.simd integration

if(Boost_FOUND AND Boost_VERSION GREATER 106099)
    # Boost.SIMD has problems building with gcc-7.x
    if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
        make_test(boost.simd.cpp target "-Wno-conversion -DCNL_BOOST_SIMD_ENABLED")

        ExternalProject_Add(
            boost.simd
            URL "https://github.com/numscale/boost.simd/archive/b1e046e3ffa720226e45512c76c505f34cde7be2.zip"
            URL_MD5 "74fc0bc279a18f931e4d2ec026af8dd4"
            UPDATE_COMMAND ""
            INSTALL_COMMAND ""
        )
        add_dependencies("${target}" boost.simd)

        ExternalProject_Get_Property(boost.simd source_dir)
        target_include_directories("${target}" PRIVATE SYSTEM "${source_dir}/include")
    endif(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
endif(Boost_FOUND AND Boost_VERSION GREATER 106099)
