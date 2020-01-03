include("../common/common.cmake")

enable_testing()

# source files containing tests of the correctness of CNL;
# ideally compiles with every warning that CNL user might choose
set(test_sources
        # test cnl/all.h
        all.cpp

        # free functions
        bit.cpp
        cmath.cpp
        cstdint.cpp
        common.cpp
        limits.cpp
        math.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        overflow/overflow.cpp
        overflow/rounding/integer.cpp
        rounding/rounding.cpp
        _impl/num_traits/max_digits.cpp
        _impl/num_traits/adopt.cpp
        _impl/num_traits/adopt_digits.cpp
        _impl/num_traits/adopt_signedness.cpp
        _impl/overflow/is_overflow.cpp
        _impl/rounding/convert_operator.cpp

        # components
        constant.cpp
        integer/digits.cpp
        integer/set_rep.cpp
        integer/from_value.cpp
        integer/make_integer.cpp
        integer/numeric_limits.cpp
        integer/operators.cpp
        integer/scale.cpp
        integer/set_digits.cpp
        integer/type.cpp
        scaled_integer/scaled_integer_built_in.cpp
        scaled_integer/decimal.cpp
        scaled_integer/constants.cpp
        fraction/ctors.cpp
        fraction/fraction.cpp
        elastic_integer/elastic_integer.cpp
        scaled_integer/extras.cpp
        overflow/overflow_integer.cpp
        overflow/overflow_tag.cpp
        rounding/rounding_integer.cpp
        _impl/duplex_integer/digits.cpp
        _impl/duplex_integer/numeric_limits.cpp
        _impl/duplex_integer/operators.cpp
        _impl/duplex_integer/instantiate_duplex_integer.cpp
        _impl/duplex_integer/type.cpp
        _impl/wide_integer/digits.cpp
        _impl/wide_integer/from_rep.cpp
        _impl/wide_integer/from_value.cpp
        _impl/wide_integer/literals.cpp
        _impl/wide_integer/make_wide_integer.cpp
        _impl/wide_integer/numeric_limits.cpp
        _impl/wide_integer/scale.cpp
        _impl/wide_integer/set_digits.cpp
        _impl/wide_integer/definition.cpp
        _impl/wide_integer/generic.cpp

        # components in free functions
        elastic_integer/rounding_integer/rounding_elastic_integer.cpp
        rounding/fixed_point/elastic_integer/elastic_fixed_point.cpp
        rounding/fixed_point/fixed_point.cpp
        rounding/scaled_integer/elastic_integer/elastic_scaled_integer.cpp
        rounding/scaled_integer/scaled_integer.cpp

        # composites
        elastic_integer/wide_integer/wide_elastic_integer.cpp
        scaled_integer/integer/scaled_integer_integer.cpp
        scaled_integer/overflow/scaled_integer_native_integer.cpp
        scaled_integer/overflow/scaled_integer_trapping_integer.cpp
        scaled_integer/overflow/scaled_integer_throwing_integer.cpp
        scaled_integer/overflow/scaled_integer_saturated_integer.cpp
        scaled_integer/rounding/scaled_integer_rounding.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer_32.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer_8.cpp
        scaled_integer/overflow/undefined_overflow.cpp
        scaled_integer/rounding/overflow/scaled_integer_rounding_overflow.cpp
        scaled_integer/rounding/elastic/integer.cpp
        scaled_integer/elastic/rounding/overflow/426.cpp
        rounding/elastic_integer/elastic_integer.cpp
        rounding/overflow/elastic/integer.cpp
        rounding/wide_integer/wide_integer.cpp
        scaled_integer/rounding/rounding_scaled_integer.cpp
        elastic_integer/rounding_integer/set_rounding.cpp
        elastic_integer/rounding_integer/overflow_integer/rounding_safe_integer.cpp
        overflow/elastic/safe_integer.cpp
        overflow/wide/wide_overflow_integer.cpp
        scaled_integer/elastic/make_elastic_scaled_integer.cpp
        scaled_integer/elastic/elastic_scaled_integer.cpp
        scaled_integer/rounding/elastic/rounding_elastic_scaled_integer.cpp
        scaled_integer/overflow/elastic/integer.cpp
        static_integer/operators.cpp
        static_integer/type.cpp
        static_number/426.cpp
        static_number/429.cpp
        static_number/operators.cpp
        static_number/rounding.cpp
        static_number/to_chars.cpp
        static_number/type.cpp
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
        scaled_integer/math.cpp
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

if(Boost_FOUND)
    if(NOT Boost_VERSION GREATER 105500)
        message("Boost version is too low to use with CNL")
    endif(NOT Boost_VERSION GREATER 105500)
endif(Boost_FOUND)

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
    target_link_libraries("${target}" ${CONAN_LIBS})

    target_include_directories("${target}" SYSTEM PRIVATE "${CONAN_INCLUDE_DIRS_GTEST}")
endfunction(add_gtest_dependency)

######################################################################
# Tests - the full suite of tests

add_custom_target(Tests)

######################################################################
# test plain - the main.cpp project with *no* tests of compiler flags

add_executable(TestPlain all.cpp)
set_target_properties(TestPlain PROPERTIES COMPILE_FLAGS "${STD_FLAGS}")
target_link_libraries(TestPlain Cnl)
add_test(TestPlain "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestPlain")
add_gtest_dependency(TestPlain)
add_dependencies(Tests TestPlain)

######################################################################
# ClangTidy

set(CNL_CLANG_TIDY OFF CACHE BOOL "create Clang-Tidy tests")

find_program(ClangTidy clang-tidy)

if (${ClangTidy} STREQUAL "ClangTidy-NOTFOUND")
    message(STATUS "ClangTidy not found.")
endif (${ClangTidy} STREQUAL "ClangTidy-NOTFOUND")

######################################################################
# make_test

# given the path to a source file containing gtest tests,
# creates and returns a target that runs them
function(make_test source out_target compile_flags linker_flags)

    # turn source file path into target name (e.g. "foo/bar.cpp" -> "test-foo-bar")
    string(REPLACE ".cpp" "" stripped "${source}")
    string(REPLACE "/" "-" target "Test-${stripped}")

    # create a target and make it a test
    add_executable("${target}" "${source}")
    target_include_directories("${target}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
    set_target_properties("${target}" PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${compile_flags}")
    target_link_libraries("${target}" Cnl ${linker_flags})
    add_test("${target}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}")

    # Google Test dependency
    add_gtest_dependency("${target}")

    # Boost dependency
    if(Boost_FOUND)
        if(Boost_VERSION GREATER 105500)
            # only compile multiprecision.cpp if Boost.Multiprecision is available
            target_compile_definitions("${target}" PRIVATE "-DCNL_BOOST_ENABLED")
            target_include_directories("${target}" SYSTEM PRIVATE "${Boost_INCLUDE_DIRS}")
        endif(Boost_VERSION GREATER 105500)
    endif(Boost_FOUND)

    # Clang-Tidy test
    if (${CNL_CLANG_TIDY})
        string(REPLACE "/" "-" clang_tidy_target "Tidy-${stripped}")
        if (NOT ${ClangTidy} STREQUAL "ClangTidy-NOTFOUND")
            add_test(
                    "${clang_tidy_target}"
                    "${ClangTidy}"
                    "-p=${CMAKE_BINARY_DIR}"
                    "-extra-arg=-Wno-unknown-warning-option"
                    "-header-filter=include/cnl"
                    "${CMAKE_CURRENT_LIST_DIR}/${source}")
        endif (NOT ${ClangTidy} STREQUAL "ClangTidy-NOTFOUND")
    endif (${CNL_CLANG_TIDY})

    # Add to Tests custom target.
    add_dependencies(Tests "${target}")

    # Return name of test target.
    set(${out_target} "${target}" PARENT_SCOPE)
endfunction(make_test)

######################################################################
# create tests to verify CNL

if (IS_MSVC)
    set(SANITIZE_ENABLED_CXX_FLAGS "")
elseif (IS_CLANG)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.8.0")
        set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize=address")
        set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=undefined --rtlib=compiler-rt -lasan")
    else ()
        set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize-trap=undefined -fsanitize=address")
        set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address -rtlib=compiler-rt -lgcc_s")
    endif ()
else ()
    set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=undefined -fsanitize-undefined-trap-on-error -fsanitize=address")
    set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address -static-libasan")
endif ()

if (IS_MSVC)
    set(TEST_CXX_FLAGS "")
else ()
    set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
endif ()

set(CNL_SANITIZE ON CACHE BOOL "compile with UB sanitizer enabled")
if (CNL_SANITIZE)
    set(SANITIZE_CXX_FLAGS "${SANITIZE_ENABLED_CXX_FLAGS}")
    set(SANITIZE_LINKER_FLAGS "${SANITIZE_ENABLED_LINKER_FLAGS}")
endif (CNL_SANITIZE)

foreach(source ${test_sources})
    make_test("${source}" target "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}" "${SANITIZE_LINKER_FLAGS}")
endforeach(source)

######################################################################
# create tests to show off CNL

foreach(source ${sample_sources})
    if (IS_CLANG_FAMILY OR IS_GCC_FAMILY)
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

if(Boost_FOUND AND Boost_VERSION GREATER 106099 AND CNL_EXCEPTIONS)
    # Boost.SIMD has problems building with gcc-7.x
    if(NOT IS_GCC_FAMILY OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
        if (IS_CLANG_FAMILY OR IS_GCC_FAMILY)
            make_test(boost.simd.cpp target "-Wno-conversion" "")
        else ()
            make_test(boost.simd.cpp target "" "")
        endif ()

        ExternalProject_Add(
            BoostSimd
            URL "https://github.com/johnmcfarlane/boost.simd/archive/cnl.zip"
            URL_MD5 "29e0e22801fb8cfb7742005c89183bba"
            UPDATE_COMMAND ""
            INSTALL_COMMAND ""
            CMAKE_ARGS "-DBoost_INCLUDE_DIR=${Boost_INCLUDE_DIR}"
        )
        add_dependencies("${target}" BoostSimd)

        ExternalProject_Get_Property(BoostSimd source_dir)
        target_include_directories("${target}" PRIVATE SYSTEM "${source_dir}/include")
    endif(NOT IS_GCC_FAMILY OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
endif(Boost_FOUND AND Boost_VERSION GREATER 106099 AND CNL_EXCEPTIONS)

######################################################################
# create test of Vc integration

set(CNL_TEST_VC ON CACHE BOOL "test VC library integration")
if(${CNL_TEST_VC} AND ${CNL_STD} STREQUAL "17" AND IS_GCC_FAMILY AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0.0"
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
