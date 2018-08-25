
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/limits.h>
#include <cnl/numeric.h>

#include <cnl/_impl/common.h>
#include <cnl/_impl/type_traits/identical.h>

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// test specific operations

// Lhs == Rhs
template<class Lhs, class Rhs>
constexpr bool is_equal_to(Lhs const& lhs, Rhs const& rhs)
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
constexpr bool is_less_than(Lesser const& lesser, Greater const& greater)
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

using cnl::_impl::identical;

// performs tests that should pass for all numeric types (except maybe const_integer);
// invokes specific tests that only pass for subject types 
template<class Number>
struct number_test {
    using value_type = Number;
    using numeric_limits = cnl::numeric_limits<value_type>;

    static constexpr value_type zero = cnl::from_rep<value_type>{}(0);
#if defined(_MSC_VER)
    static constexpr value_type negative_zero{ zero };
#else
    static constexpr value_type negative_zero{ -zero };
#endif    

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits

    static_assert(numeric_limits::is_specialized, "numeric_limits is not specialized for this type");

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
    static_assert(is_equal_to(static_cast<double>(zero), 0.), "comparison test error");

    // comparisons between zero and zero-initialized value
    static_assert(is_equal_to(zero, value_type(0.)), "zero-initialized value is not represented using zero");

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::width / cnl::_impl::set_width_t

    static_assert(cnl::digits<value_type>::value
                    ==cnl::digits<cnl::set_digits_t<value_type, cnl::digits<value_type>::value>>::value,
            "cnl::digits / cnl::set_digits test failed");

    static_assert(cnl::digits<cnl::set_digits_t<value_type, 3>>::value>=3, "cnl::digits / cnl::set_digits test failed");
    static_assert(cnl::digits<cnl::set_digits_t<value_type, 9>>::value>=9, "cnl::digits / cnl::set_digits test failed");
    static_assert(cnl::digits<cnl::set_digits_t<value_type, 63>>::value>32,
            "cnl::digits / cnl::set_digits test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // test operator+

    // TODO: arithmetic failing for GCC 7
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
    // numeric traits

    // would not pass for boost.multiprecision
    static_assert(cnl::is_composite<value_type>::value != std::is_fundamental<value_type>::value, "is_composite test failed");

    static constexpr auto lowest_from_rep = cnl::from_rep<value_type>{}(cnl::_impl::to_rep(lowest));
    static_assert(identical(lowest_from_rep, lowest), "cnl::_impl::to_rep & from_rep test failed");

    static constexpr auto zero_from_rep = cnl::from_rep<value_type>{}(cnl::_impl::to_rep(zero));
    static_assert(identical(zero_from_rep, zero), "cnl::_impl::to_rep & from_rep test failed");

    static constexpr auto max_from_rep = cnl::from_rep<value_type>{}(cnl::_impl::to_rep(max));
    static_assert(identical(max_from_rep, max), "cnl::_impl::to_rep & from_rep test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // bit functions

    static_assert(cnl::used_digits(zero)==0, "used_digits test failed");
    static_assert(cnl::used_digits(max)==cnl::digits<value_type>::value, "used_digits test failed");

    static_assert(cnl::leading_bits(zero)==cnl::digits<value_type>::value, "leading_bits test failed");
    static_assert(cnl::leading_bits(max)==0, "leading_bits test failed");
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
#if defined(CNL_INT128)
          number_test_suite<NumericType<CNL_INT128>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<CNL_UINT128>, TypeSpecificTestSuite>,
#endif
          number_test_suite<NumericType<cnl::int8>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::uint8>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::int16>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::uint16>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::int32>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::uint32>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::int64>, TypeSpecificTestSuite>,
          number_test_suite<NumericType<cnl::uint64>, TypeSpecificTestSuite>
{
};

// given a rounding tag, invokes number_test_suite for integers of all built-in types
template<template <class, class> class NumericType, class Tag, template<class> class TypeSpecificTestSuite = std::is_integral>
struct number_test_by_rep_by_tag {
    template<class Rep>
    using test_subject = NumericType<Rep, Tag>;

    constexpr static number_test_by_rep<test_subject, TypeSpecificTestSuite> instance{};
};
