
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

template<typename Rep, int Exponent>
using decimal_fixed_point = cnl::fixed_point<Rep, Exponent, 10>;

using cnl::_impl::identical;

namespace test_ctor_int {
    static_assert(identical(decimal_fixed_point<int, 1>{10}, cnl::from_rep<decimal_fixed_point<int, 1>>{}(1)), "");
    static_assert(identical(1, cnl::to_rep(decimal_fixed_point<int, 1>{10})), "");
}

namespace test_ctor_float {
    static_assert(identical(decimal_fixed_point<int, 1>{10.}, cnl::from_rep<decimal_fixed_point<int, 1>>{}(1)), "");
    static_assert(identical(1, cnl::to_rep(decimal_fixed_point<int, 1>{10.})), "");

    static_assert(identical(decimal_fixed_point<int, -1>{1.5}, cnl::from_rep<decimal_fixed_point<int, -1>>{}(15)), "");
    static_assert(identical(15, cnl::to_rep(decimal_fixed_point<int, -1>{1.5})), "");

    static_assert(identical(decimal_fixed_point<int, -1>{5.1}, cnl::from_rep<decimal_fixed_point<int, -1>>{}(51)), "");
    static_assert(identical(51, cnl::to_rep(decimal_fixed_point<int, -1>{5.1})), "");
}

namespace test_add {
    static_assert(identical(
            decimal_fixed_point<int, -5>{1.5}+decimal_fixed_point<int, -5>{5.1},
            decimal_fixed_point<int, -5>{6.59998989105224609375f}), "");
}

namespace test_multiply {
    static_assert(identical(
            decimal_fixed_point<int, -1>{7.2}*decimal_fixed_point<int, -3>{321.123},
            decimal_fixed_point<int, -4>{2312.08559999999988576746545732021331787109375}), "");
}

namespace test_divide {
    static_assert(identical(
            decimal_fixed_point<int, -3>{1}/decimal_fixed_point<int, 0>{3},
            decimal_fixed_point<int, -3>{0.333}), "");

    static_assert(identical(
            decimal_fixed_point<int, -3>{2}/decimal_fixed_point<int, 0>{3},
            decimal_fixed_point<int, -3>{0.666}), "");
}

namespace test_remainder {
    static_assert(identical(
            decimal_fixed_point<int, -3>{1}%decimal_fixed_point<int, 0>{3},
            decimal_fixed_point<int, -3>{0.001}), "");

    static_assert(identical(
            decimal_fixed_point<int, -3>{2}%decimal_fixed_point<int, 0>{3},
            decimal_fixed_point<int, -3>{0.002}), "");
}
