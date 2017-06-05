
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/bits/limits.h>
#include <sg14/numeric_traits>

#include <sg14/bits/common.h>

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// test specific operations

// Lhs == Rhs
template<class Lhs, class Rhs>
constexpr bool is_equal_to(const Lhs& lhs, const Rhs& rhs)
{
    return ((lhs==rhs)==true)
            && ((lhs!=rhs)==false)
            && ((lhs<rhs)==false)
            && ((lhs>rhs)==false)
            && ((lhs<=rhs)==true)
            && ((lhs>=rhs)==true);
}

static_assert(is_equal_to<int>(0, 0), "less_than_test test failed");

// lesser < greater
template<class Lesser, class Greater>
constexpr bool is_less_than(const Lesser& lesser, const Greater& greater)
{
    return ((lesser==greater)==false)
            && ((lesser!=greater)==true)
            && ((lesser<greater)==true)
            && ((lesser>greater)==false)
            && ((lesser<=greater)==true)
            && ((lesser>=greater)==false);
}

static_assert(is_less_than<int>(0, 1), "less_than_test test failed");

////////////////////////////////////////////////////////////////////////////////
// number_test

using sg14::_impl::identical;

// performs tests that should pass for all numeric types (except maybe const_integer);
// invokes specific tests that only pass for subject types 
template<class Number>
struct number_test {
    using value_type = Number;
    using numeric_limits = std::numeric_limits<value_type>;
    using numeric_traits = sg14::numeric_traits<value_type>;
    
    static constexpr value_type zero = numeric_traits::from_rep(0);
#if defined(_MSC_VER)
    static constexpr value_type negative_zero{ zero };
#else
    static constexpr value_type negative_zero{ -zero };
#endif    

    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits

    static_assert(numeric_limits::is_specialized, "std::numeric_limits is not specialized for this type");

    static constexpr value_type min{numeric_limits::min()};
    static constexpr value_type max{numeric_limits::max()};
    static constexpr value_type lowest{numeric_limits::lowest()};

    static_assert(is_less_than(lowest, max), "numeric_limits test failed");

    static_assert(!is_less_than(max, lowest), "numeric_limits test failed");
    static_assert(!is_less_than(zero, lowest), "numeric_limits test failed");
    static_assert(!is_less_than(min, lowest), "numeric_limits test failed");
    static_assert(!is_less_than(lowest, lowest), "numeric_limits test failed");

    static_assert(!is_less_than(max, max), "numeric_limits test failed");
    static_assert(!is_less_than(max, lowest), "numeric_limits test failed");
    static_assert(!is_less_than(max, min), "numeric_limits test failed");
    static_assert(!is_less_than(max, zero), "numeric_limits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test comparison operators

    // comparisons between minimum value
    static_assert(is_equal_to(lowest, lowest), "comparison test error");

    // comparisons between zero
    static_assert(is_equal_to(zero, zero), "comparison test error");

    // comparisons between minimum value
    static_assert(is_equal_to(max, max), "comparison test error");

    // comparisons between zero and literal zero
    static_assert(is_equal_to(zero, 0.), "comparison test error");

    // comparisons between zero and zero-initialized value
    static_assert(is_equal_to(zero, value_type(0.)), "zero-initialized value is not represented using zero");

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::width / sg14::_impl::set_width_t

    static_assert(sg14::numeric_traits<value_type>::digits
                    ==sg14::numeric_traits<sg14::_impl::set_digits_t<value_type, sg14::numeric_traits<value_type>::digits>>::digits,
            "sg14::width / sg14::set_width test failed");

    static_assert(sg14::numeric_traits<sg14::_impl::set_digits_t<value_type, 3>>::digits>=3, "sg14::digits / sg14::set_digits test failed");
    static_assert(sg14::numeric_traits<sg14::_impl::set_digits_t<value_type, 9>>::digits>=9, "sg14::digits / sg14::set_digits test failed");
    static_assert(sg14::numeric_traits<sg14::_impl::set_digits_t<value_type, 63>>::digits>32,
            "sg14::digits / sg14::set_digits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    static_assert(zero+zero==zero, "operator+ test failed");
    static_assert(zero+zero+zero==zero, "operator+ test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator-

    static_assert(is_equal_to(zero-zero, zero), "operator- test failed");
    static_assert(is_equal_to(zero-zero-zero, zero), "operator- test failed");

    static_assert(is_equal_to(min-min, zero), "operator- test failed");
    static_assert(is_equal_to(min-zero, min), "operator- test failed");

    static_assert(is_equal_to(max-max, zero), "operator- test failed");
    
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::numeric_traits

    static_assert(
            std::is_same<typename numeric_traits::value_type, value_type>::value,
            "numeric_traits::value_type test failed");
    static_assert(
            numeric_traits::is_specialized,
            "numeric_traits::is_specialized test failed");

    static constexpr auto lowest_from_rep = numeric_traits::from_rep(numeric_traits::to_rep(lowest));
    static_assert(identical(lowest_from_rep, lowest), "numeric_traits::to_rep & from_rep test failed");

    static constexpr auto zero_from_rep = numeric_traits::from_rep(numeric_traits::to_rep(zero));
    static_assert(identical(zero_from_rep, zero), "numeric_traits::to_rep & from_rep test failed");

    static constexpr auto max_from_rep = numeric_traits::from_rep(numeric_traits::to_rep(max));
    static_assert(identical(max_from_rep, max), "numeric_traits::to_rep & from_rep test failed");
};

// performs tests that should pass for all numeric types (except maybe const_integer);
// invokes specific tests that only pass for subject types
template<class Number, template<class> class TypeSpecificTestSuite>
struct number_test_suite
        : number_test<Number>
        , TypeSpecificTestSuite<Number> {
};

template<template<class> class NumericType, template<class> class TypeSpecificTestSuite = std::is_integral>
struct number_test_by_rep
        : number_test_suite<NumericType<char>, TypeSpecificTestSuite>,
#if defined(SG14_INT128)
        number_test_suite<NumericType<SG14_INT128>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<SG14_UINT128>, TypeSpecificTestSuite>,
#endif
          number_test_suite<NumericType<std::int8_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::uint8_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::int16_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::uint16_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::int32_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::uint32_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::int64_t>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<std::uint64_t>, TypeSpecificTestSuite>
{
};

// given a rounding policy, invokes number_test_suite for precise_integers of all built-in types
template<template <class, class> class NumericType, class Policy, template<class> class TypeSpecificTestSuite = std::is_integral>
struct number_test_by_rep_by_policy {
    template<class Rep>
    using test_subject = NumericType<Rep, Policy>;

    constexpr static number_test_by_rep<test_subject, TypeSpecificTestSuite> instance{};
};
