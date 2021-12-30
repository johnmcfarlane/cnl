enable_testing()

# source files containing tests of the correctness of CNL;
# ideally compiles with every warning that CNL user might choose
set(test_sources
        # slowest
        static_int/operators.cpp

        # test cnl/all.h
        all.cpp

        # free functions
        bit.cpp
        cmath.cpp
        cstdint.cpp
        fixed_point.cpp
        int.cpp
        narrow_cast.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        overflow/overflow.cpp
        overflow/rounding/int.cpp
        rounding/rounding.cpp
        _impl/cmath/abs.cpp
        _impl/cmath/sqrt.cpp
        _impl/elastic_int/sqrt.cpp
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
        wrapper/declaration.cpp
        scaled_int/scaled_int_built_in.cpp
        scaled_int/decimal.cpp
        scaled_int/numbers.cpp
        fraction/ctors.cpp
        fraction/fraction.cpp
        elastic_int/elastic_int.cpp
        scaled_int/extras.cpp
        overflow/overflow_int.cpp
        overflow/overflow_tag.cpp
        rounding/rounding_int.cpp
        _impl/duplex_int/definition.cpp
        _impl/duplex_int/digits.cpp
        _impl/duplex_int/from_value.cpp
        _impl/duplex_int/numeric_limits.cpp
        _impl/duplex_int/operators.cpp
        _impl/duplex_int/narrowest_int.cpp
        _impl/wide_int/digits.cpp
        _impl/wide_int/from_rep.cpp
        _impl/wide_int/from_value.cpp
        _impl/wide_int/literals.cpp
        _impl/wide_int/make_wide_int.cpp
        _impl/wide_int/numeric_limits.cpp
        _impl/wide_int/scale.cpp
        _impl/wide_int/set_digits.cpp
        _impl/wide_int/definition.cpp
        _impl/wide_int/generic.cpp

        # components in free functions
        elastic_int/rounding_int/rounding_elastic_int.cpp
        rounding/scaled_int/elastic_int/native.cpp
        rounding/scaled_int/elastic_int/tie_to_pos_inf.cpp
        rounding/scaled_int/elastic_int/neg_inf.cpp
        rounding/scaled_int/elastic_int/nearest.cpp
        rounding/scaled_int/scaled_int.cpp

        # composites
        elastic_int/wide_int/wide_elastic_int.cpp
        scaled_int/overflow/scaled_int_native_int.cpp
        scaled_int/overflow/scaled_int_trapping_int.cpp
        scaled_int/overflow/scaled_int_throwing_int.cpp
        scaled_int/overflow/scaled_int_saturated_int.cpp
        scaled_int/rounding/scaled_int_rounding.cpp
        scaled_int/wide_int/scaled_int_wide_int.cpp
        scaled_int/wide_int/scaled_int_wide_int_32.cpp
        scaled_int/wide_int/scaled_int_wide_int_8.cpp
        scaled_int/wrapper/scaled_int_wrapper.cpp
        scaled_int/overflow/undefined_overflow.cpp
        scaled_int/rounding/overflow/native.cpp
        scaled_int/rounding/elastic/int.cpp
        scaled_int/elastic/rounding/overflow/426.cpp
        rounding/elastic_int/elastic_int.cpp
        rounding/overflow/elastic/int.cpp
        rounding/wide_int/wide_int.cpp
        scaled_int/rounding/rounding_scaled_int.cpp
        elastic_int/rounding_int/set_rounding.cpp
        elastic_int/rounding_int/overflow_int/safe.cpp
        overflow/elastic/safe_int.cpp
        overflow/wide/wide_overflow_int.cpp
        scaled_int/elastic/make_elastic_scaled_int.cpp
        scaled_int/elastic/elastic_scaled_int.cpp
        scaled_int/rounding/elastic/rounding_elastic_scaled_int.cpp
        scaled_int/overflow/elastic/int.cpp
        static_int/shift_left.cpp
        static_int/type.cpp
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
)

######################################################################
# pull in external dependencies

find_package(Boost 1.71.0)

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
target_link_libraries(test-unit-plain Cnl)
add_test(test-unit-plain "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-unit-plain")
add_gtest_dependency(test-unit-plain)
add_dependencies(test-unit test-unit-plain)

######################################################################
# make_test

# given the path to a source file containing gtest tests,
# creates and returns a target that runs them
function(make_test source compile_flags)

    # turn source file path into target name (e.g. "foo/bar.cpp" -> "test-unit-foo-bar")
    string(REPLACE ".cpp" "" stripped "${source}")
    string(REPLACE "/" "-" target "test-unit-${stripped}")

    # create a target and make it a test
    add_executable("${target}" "${source}")
    target_include_directories("${target}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
    target_compile_options("${target}" PRIVATE "${compile_flags}")
    target_link_libraries("${target}" Cnl)
    add_test("${target}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target}")

    # Google Test dependency
    add_gtest_dependency("${target}")

    # Boost dependency
    if(Boost_FOUND)
        target_compile_definitions("${target}" PRIVATE "-DCNL_BOOST_ENABLED")
        target_include_directories("${target}" SYSTEM PRIVATE "${Boost_INCLUDE_DIRS}")
    endif(Boost_FOUND)

    # Add to test-unit custom target.
    add_dependencies(test-unit "${target}")
endfunction(make_test)

######################################################################
# create tests to verify CNL

foreach(source ${test_sources})
    make_test("${source}" "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}")
endforeach(source)

if (CNL_SANITIZE)
    make_test(ub.cpp "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}")
endif(CNL_SANITIZE)

######################################################################
# create tests to show off CNL

foreach(source ${sample_sources})
    make_test("${source}" "${SAMPLE_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}")
endforeach(source)

make_test(scaled_int/math.cpp "${SAMPLE_CXX_FLAGS}")

######################################################################
# make test of Boost.Multiprecision integration

if(Boost_FOUND)
    make_test(boost.multiprecision.cpp "${TEST_CXX_FLAGS} ${SANITIZE_CXX_FLAGS}")
endif(Boost_FOUND)
