
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/precise_integer.h>

#include "number_test.h"

#include <gtest/gtest.h>

namespace {
    using sg14::precise_integer;
    using std::is_same;
    using sg14::_impl::identical;

    namespace number_base_traits {
        using sg14::_impl::number_base;
        using sg14::_impl::is_derived_from_number_base;

        static_assert(!is_derived_from_number_base<int>::value, "");
        static_assert(!is_derived_from_number_base<number_base<precise_integer<>, int>>::value, "");
        static_assert(is_derived_from_number_base<precise_integer<>>::value, "");
    }

    namespace default_parameters {
        using sg14::closest_rounding_policy;

        using default_rep = int;

        template<typename T>
        using default_policy = sg14::closest_rounding_policy;

        static_assert(is_same<precise_integer<>, precise_integer<default_rep, default_policy<default_rep>>>::value, "sg14::precise_integer parameter default test failed");

        static_assert(is_same<precise_integer<>::rep, default_rep>::value, "sg14::precise_integer parameter default test failed");
        static_assert(is_same<precise_integer<>::rounding, default_policy<default_rep>>::value, "sg14::precise_integer parameter default test failed");
    }

    namespace is_number {
        using sg14::_impl::is_derived_from_number_base;

        static_assert(is_derived_from_number_base<precise_integer<>>::value, "is_derived_from_number_base<precise_integer<>>");
    }
    
    namespace test_operate {
        using namespace sg14::_impl;
        static_assert(
                operate(precise_integer<>{2468}, precise_integer<>{2468}, equal_tag),
                "sg14::numeric_traits<precise_integer> test failed");
    }

    namespace test_numeric_traits {
        using sg14::numeric_traits;

        static_assert(
                identical(123, numeric_traits<precise_integer<>>::to_rep(precise_integer<>{123})),
                "sg14::numeric_traits<precise_integer> test failed");
        static_assert(
                identical(precise_integer<>{2468}, precise_integer<>{2468}),
                "sg14::numeric_traits<precise_integer> test failed");
        static_assert(
                identical(precise_integer<>{2468}, numeric_traits<precise_integer<>>::from_rep(2468)),
                "sg14::numeric_traits<precise_integer> test failed");
    }

    namespace test_conversion_operator {
        static_assert(identical(static_cast<int>(precise_integer<>{9876}), 9876), "precise_integer conversion test failed");
    }

    namespace test_closest_rounding_policy {
        using sg14::closest_rounding_policy;
        static_assert(identical(closest_rounding_policy::convert<int>(0.), 0), "sg14::closest_rounding_policy test failed");
        static_assert(identical(closest_rounding_policy::convert<int>(-1.), -1), "sg14::closest_rounding_policy test failed");
    }

    namespace closest {
        using precise_integer = sg14::precise_integer<>;

        static_assert(precise_integer{0.} == 0, "sg14::precise_integer test failed");
        static_assert(precise_integer{1.} == 1, "sg14::precise_integer test failed");
        static_assert(precise_integer{-1.} == -1, "sg14::precise_integer test failed");
        static_assert(precise_integer{.5} == 1, "sg14::precise_integer test failed");
        static_assert(precise_integer{-.5} == -1, "sg14::precise_integer test failed");
        static_assert(precise_integer{0.499} == 0, "sg14::precise_integer test failed");
        static_assert(precise_integer{-0.501} == -1, "sg14::precise_integer test failed");
    }

    namespace arithmetic {
        static_assert(identical(precise_integer<>{3}*precise_integer<>{7}, precise_integer<>{21}),
                "precise_integer operator*");
    }
}

template<class PreciseInteger>
struct precise_integer_tests {
    // from_rep
    static_assert(identical(precise_integer<>{123}, sg14::_impl::from_rep<precise_integer<>>(123)), "sg14::_impl::from_rep<precise_integer<>> test failed");

    // to_rep
    static_assert(identical(123, sg14::_impl::to_rep(123)), "sg14::_impl::to_rep test failed");
    static_assert(identical(321, to_rep(precise_integer<>{321})), "sg14::_impl::to_rep test failed");
};

template struct number_test_by_rep_by_policy<precise_integer, sg14::closest_rounding_policy, precise_integer_tests>;
