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
        limits.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        overflow/overflow.cpp
        overflow/rounding/integer.cpp
        rounding/rounding.cpp
        _impl/cmath/abs.cpp
        _impl/cmath/sqrt.cpp
        _impl/elastic_integer/sqrt.cpp
        _impl/num_traits/max_digits.cpp
        _impl/num_traits/adopt_digits.cpp
        _impl/numbers/adopt_signedness.cpp
        _impl/ostream.cpp
        _impl/overflow/is_overflow.cpp
        _impl/rounding/convert_operator.cpp

        # components
        constant.cpp
        wrapper/digits.cpp
        wrapper/set_rep.cpp
        wrapper/from_value.cpp
        wrapper/make_wrapper.cpp
        wrapper/numeric_limits.cpp
        wrapper/operators.cpp
        wrapper/scale.cpp
        wrapper/set_digits.cpp
        wrapper/type.cpp
        scaled_integer/scaled_integer_built_in.cpp
        scaled_integer/decimal.cpp
        scaled_integer/numbers.cpp
        fraction/ctors.cpp
        fraction/fraction.cpp
        elastic_integer/elastic_integer.cpp
        scaled_integer/extras.cpp
        overflow/overflow_integer.cpp
        overflow/overflow_tag.cpp
        rounding/rounding_integer.cpp
        _impl/duplex_integer/digits.cpp
        _impl/duplex_integer/from_value.cpp
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
        scaled_integer/overflow/scaled_integer_native_integer.cpp
        scaled_integer/overflow/scaled_integer_trapping_integer.cpp
        scaled_integer/overflow/scaled_integer_throwing_integer.cpp
        scaled_integer/overflow/scaled_integer_saturated_integer.cpp
        scaled_integer/rounding/scaled_integer_rounding.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer_32.cpp
        scaled_integer/wide_integer/scaled_integer_wide_integer_8.cpp
        scaled_integer/wrapper/scaled_integer_wrapper.cpp
        scaled_integer/overflow/undefined_overflow.cpp
        scaled_integer/rounding/overflow/scaled_integer_rounding_overflow.cpp
        scaled_integer/rounding/elastic/integer.cpp
        scaled_integer/elastic/rounding/overflow/426.cpp
        rounding/elastic_integer/elastic_integer.cpp
        rounding/fixed_point/elastic_integer/elastic_fixed_point_tie_to_pos_inf.cpp
        rounding/fixed_point/elastic_integer/elastic_fixed_point_neg_inf.cpp
        rounding/fixed_point/elastic_integer/elastic_fixed_point_nearest.cpp
        rounding/overflow/elastic/integer.cpp
        rounding/rounding_integer/rounding_integer.cpp
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
        zero_cost_square.cpp
        zero_cost_free_functions.cpp
        snippets.cpp
        zero_cost_average.cpp
        boost.multiprecision.cpp
)

######################################################################
# pull in external dependencies

find_package(Boost 1.55.0)

######################################################################
# add_gtest_dependency

function(add_gtest_dependency target)
    target_link_libraries("${target}" ${CNL_GTEST_MAIN_TARGET})
endfunction(add_gtest_dependency)

######################################################################
# add test-unit target

add_custom_target(test-unit)
add_dependencies(test-all test-unit)

######################################################################
# test plain - the all.cpp project with *no* tests of compiler flags

add_executable(test-unit-plain all.cpp)
set_target_properties(test-unit-plain PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}")
target_link_libraries(test-unit-plain "${COMMON_LINK_FLAGS}")
add_test(test-unit-plain "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-unit-plain")
add_gtest_dependency(test-unit-plain)
add_dependencies(test-unit test-unit-plain)

######################################################################
# make_test

# given the path to a source file containing gtest tests,
# creates and returns a target that runs them
function(make_test source out_target compile_flags linker_flags)

    # turn source file path into target name (e.g. "foo/bar.cpp" -> "test-unit-foo-bar")
    string(REPLACE ".cpp" "" stripped "${source}")
    string(REPLACE "/" "-" target "test-unit-${stripped}")

    # create a target and make it a test
    add_executable("${target}" "${source}")
    target_include_directories("${target}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
    set_target_properties("${target}" PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${compile_flags}")
    target_link_libraries("${target}" "${COMMON_LINK_FLAGS}" ${linker_flags})
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

    # Add to test-unit custom target.
    add_dependencies(test-unit "${target}")

    # Return name of test target.
    set(${out_target} "${target}" PARENT_SCOPE)
endfunction(make_test)

######################################################################
# create tests to verify CNL

foreach(source ${test_sources})
    make_test("${source}" target "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}" "${SANITIZE_LINKER_FLAGS}")
endforeach(source)

if (CNL_SANITIZE)
    make_test(ub.cpp target "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}" "${SANITIZE_LINKER_FLAGS}")
endif(CNL_SANITIZE)

######################################################################
# create tests to show off CNL

foreach(source ${sample_sources})
    make_test("${source}" target "${SAMPLE_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}" "${SANITIZE_LINKER_FLAGS}")
endforeach(source)

make_test(scaled_integer/math.cpp target "${SAMPLE_CXX_FLAGS}" "")
