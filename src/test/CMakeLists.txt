include("../common/common.cmake")

enable_testing()

# source files containing tests of the correctness of CNL;
# ideally compiles with every warning that CNL user might choose
set(test_sources
        # free functions
        bit.cpp
        cnlint.cpp
        common.cpp
        limits.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        readme.cpp
        overflow/overflow.cpp
        rounding/rounding.cpp

        # components
        constant.cpp
        fixed_point/fixed_point_built_in.cpp
        fixed_point/decimal.cpp
        fixed_point/constants.cpp
        fraction.cpp
        elastic/elastic_integer.cpp
        fixed_point/extras.cpp
        overflow/overflow_integer.cpp
        rounding/rounding_integer.cpp

        # components in free functions
        elastic/rounding/elastic_integer_rounding.cpp

        # composites
        fixed_point/overflow/fixed_point_native_integer.cpp
        fixed_point/overflow/fixed_point_trapping_integer.cpp
        fixed_point/overflow/fixed_point_throwing_integer.cpp
        fixed_point/overflow/fixed_point_saturated_integer.cpp
        fixed_point/rounding/fixed_point_rounding.cpp
        fixed_point/overflow/undefined_overflow.cpp
        fixed_point/rounding/overflow/fixed_point_rounding_overflow.cpp
        rounding/elastic/rounding_elastic_integer.cpp
        fixed_point/rounding/rounding_fixed_point.cpp
        elastic/rounding/overflow/rounding_safe_integer.cpp
        overflow/elastic/safe_integer.cpp
        fixed_point/elastic/make_elastic_number.cpp
        fixed_point/elastic/elastic_number.cpp
        fixed_point/elastic/rounding/rounding_elastic_number.cpp
        static_integer.cpp
        static_number.cpp
)

# source files containing demos and examples of CNL;
# do not necessarily need to fail the strictest warnings
set(sample_sources
        # showing off
        index.cpp
        papers/p0037.cpp
        papers/p0381.cpp
        papers/p0675.cpp
        papers/p0827.cpp
        papers/p0828.cpp
        papers/p0554.cpp
        papers/p1050.cpp
        presentations/cppcon2017.cpp
        presentations/cppdub2018.cpp
        presentations/cppnow2017.cpp
        fixed_point/math.cpp
        zero_cost_square.cpp
        zero_cost_free_functions.cpp
        snippets.cpp
        zero_cost_average.cpp
        boost.multiprecision.cpp
        glm.cpp
)

######################################################################
# pull in external dependencies

find_package(Boost)

ExternalProject_Add(
        GoogleTest
        URL "https://github.com/google/googletest/archive/3a4cf1a02ef4adc28fccb7eef2b573b14cd59009.zip"
        URL_MD5 "06ac495303fbe94b198026e3c196425e"
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
)

ExternalProject_Add(
        Glm
        URL "https://github.com/johnmcfarlane/glm/archive/ceb6d4fd3cb24d0ff954a6470ee554e356885d0e.zip"
        URL_MD5 "7fa4dc69b3c5a991fdf0135dbf26e22c"
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
)

######################################################################
# add_gtest_dependency

function(add_gtest_dependency target)
    # gtest
    ExternalProject_Get_Property(GoogleTest binary_dir)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        target_link_libraries(
                "${target}"
                debug ${binary_dir}/googlemock/gtest/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtestd${CMAKE_FIND_LIBRARY_SUFFIXES}
                debug ${binary_dir}/googlemock/gtest/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_maind${CMAKE_FIND_LIBRARY_SUFFIXES}
                optimized ${binary_dir}/googlemock/gtest/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
                optimized ${binary_dir}/googlemock/gtest/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    else ()
        target_link_libraries(
                "${target}"
                general "${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a"
                general "${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a"
                general pthread
        )
    endif ()

    ExternalProject_Get_Property(GoogleTest source_dir)
    target_include_directories("${target}" SYSTEM PRIVATE "${source_dir}/googletest/include")

    add_dependencies("${target}" GoogleTest)
endfunction(add_gtest_dependency)

######################################################################
# Tests - the full suite of tests

add_custom_target(Tests)

######################################################################
# make_test

# given the path to a source file containing gtest tests,
# creates and returns a target that runs them
function(make_test source out_target compile_flags linker_flags)

    # turn source file path into target name (e.g. "foo/bar.cpp" -> "test-foo-bar")
    string(REPLACE ".cpp" "" stripped "${source}")
    string(REPLACE "/" "-" target "Test-${stripped}")
    set(${out_target} "${target}" PARENT_SCOPE)

    # create a target and make it a test
    add_executable("${target}" "${source}")
    add_test("${target}" "${target}")
    set_target_properties("${target}" PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${compile_flags}")
    target_link_libraries("${target}" "Cnl" "${linker_flags}")
    target_include_directories("${target}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

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
    endif(Boost_FOUND)

    add_dependencies(Tests "${target}")
endfunction(make_test)

######################################################################
# create tests to verify CNL

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(SANITIZE_ENABLED_CXX_FLAGS "")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.8.0")
        set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize=address")
        set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=undefined --rtlib=compiler-rt -lasan")
    else ()
        set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize-trap=undefined -fsanitize=address")
        set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address")
    endif ()
else ()
    set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize-undefined-trap-on-error -fsanitize=address")
    set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address -static-libasan")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(TEST_CXX_FLAGS "")
else ()
    set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
endif ()

set(SANITIZE ON CACHE BOOL "compile with UB sanitizer enabled")
if (SANITIZE)
    set(SANITIZE_CXX_FLAGS "${SANITIZE_ENABLED_CXX_FLAGS}")
    set(SANITIZE_LINKER_FLAGS "${SANITIZE_ENABLED_LINKER_FLAGS}")
endif (SANITIZE)

foreach(source ${test_sources})
    make_test("${source}" target "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}" "${SANITIZE_LINKER_FLAGS}")
endforeach(source)

######################################################################
# create tests to show off CNL

foreach(source ${sample_sources})
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        make_test("${source}" target "-fpermissive -Wno-sign-compare -Wno-strict-overflow" "")
    else ()
        make_test("${source}" target "/wd4018" "")
    endif ()
endforeach(source)

######################################################################
# create test of GLM integration

ExternalProject_Get_Property(Glm source_dir)
target_include_directories(Test-glm SYSTEM PRIVATE "${source_dir}")
add_dependencies(Test-glm Glm)

######################################################################
# create test of Boost.SIMD integration

if(Boost_FOUND AND Boost_VERSION GREATER 106099)
    # Boost.SIMD has problems building with gcc-7.x
    if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
        if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
            make_test(boost.simd.cpp target "-Wno-conversion" "")
        else ()
            make_test(boost.simd.cpp target "" "")
        endif ()

        ExternalProject_Add(
            BoostSimd
            URL "https://github.com/numscale/boost.simd/archive/b1e046e3ffa720226e45512c76c505f34cde7be2.zip"
            URL_MD5 "74fc0bc279a18f931e4d2ec026af8dd4"
            UPDATE_COMMAND ""
            INSTALL_COMMAND ""
            CMAKE_ARGS "-DBoost_INCLUDE_DIR=${Boost_INCLUDE_DIR}"
        )
        add_dependencies("${target}" BoostSimd)

        ExternalProject_Get_Property(BoostSimd source_dir)
        target_include_directories("${target}" PRIVATE SYSTEM "${source_dir}/include")
    endif(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
endif(Boost_FOUND AND Boost_VERSION GREATER 106099)

######################################################################
# create test of Vc integration

if(${STD} STREQUAL "17" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0.0"
        AND CMAKE_VERSION VERSION_GREATER "3.2")
    make_test(vc.cpp target "" "")

    ExternalProject_Add(
            Vc
            URL "https://github.com/johnmcfarlane/Vc/archive/37ea0c6a0455ee91d62dc04d082bd4bac9fac5b3.zip"
            URL_MD5 "bab6f17c63e128b3c28ac9aa9324dd09"
            UPDATE_COMMAND ""
            INSTALL_COMMAND ""
            CMAKE_ARGS "-DBUILD_TESTING=OFF"
    )
    add_dependencies("${target}" Vc)

    ExternalProject_Get_Property(Vc source_dir)
    target_include_directories("${target}" PRIVATE SYSTEM "${source_dir}")
endif()
