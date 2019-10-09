
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/scaled_integer.h>

template<typename Rep, int Exponent>
using decimal_scaled_integer = cnl::scaled_integer<Rep, cnl::power<Exponent, 10>>;

using cnl::_impl::identical;

namespace test_ctor_int {
    static_assert(identical(decimal_scaled_integer<int, 1>{10}, cnl::_impl::from_rep<decimal_scaled_integer<int, 1>>(1)), "");
    static_assert(identical(1, cnl::_impl::to_rep(decimal_scaled_integer<int, 1>{10})), "");
}

namespace test_from_value {
    static_assert(identical(
            decimal_scaled_integer<int, 0>{123},
            cnl::_impl::from_value<decimal_scaled_integer<int, 1>>(123)), "");
}

namespace test_from_rep {
    static_assert(
            identical(decimal_scaled_integer<int, 1>{1230}, cnl::_impl::from_rep<decimal_scaled_integer<int, 1>>(123)),
            "");
}

namespace test_ctor_float {
    static_assert(
            identical(decimal_scaled_integer<int, 1>{10.}, cnl::_impl::from_rep<decimal_scaled_integer<int, 1>>(1)),
            "");
    static_assert(identical(1, cnl::_impl::to_rep(decimal_scaled_integer<int, 1>{10.})), "");

    static_assert(
            identical(decimal_scaled_integer<int, -1>{1.5}, cnl::_impl::from_rep<decimal_scaled_integer<int, -1>>(15)),
            "");
    static_assert(identical(15, cnl::_impl::to_rep(decimal_scaled_integer<int, -1>{1.5})), "");

    static_assert(
            identical(decimal_scaled_integer<int, -1>{5.1}, cnl::_impl::from_rep<decimal_scaled_integer<int, -1>>(51)),
            "");
    static_assert(identical(51, cnl::_impl::to_rep(decimal_scaled_integer<int, -1>{5.1})), "");
}

namespace test_ctor_decimal_scaled_integer {
    static_assert(identical(
            decimal_scaled_integer<int, -2>{123},
            decimal_scaled_integer<int, -2>{decimal_scaled_integer<int, 0>{123}}), "");
}

namespace test_equal {
    static_assert(decimal_scaled_integer<int, -2>{123}==123, "");
}

namespace test_add {
    static_assert(identical(
            decimal_scaled_integer<int, -5>{1.5}+decimal_scaled_integer<int, -5>{5.1},
            decimal_scaled_integer<int, -5>{6.59998989105224609375F}), "");

    static_assert(identical(
            76.5F+decimal_scaled_integer<int, -5>{1.25},
            77.75F), "");
}

namespace test_multiply {
    static_assert(identical(
            decimal_scaled_integer<int, -1>{7.2}*decimal_scaled_integer<int, -3>{321.123},
            decimal_scaled_integer<int, -4>{2312.0856}), "");

    static_assert(identical(
            decimal_scaled_integer<int, -1>{7.2}*123,
            decimal_scaled_integer<int, -1>{885.6}), "");
}

namespace test_divide {
    static_assert(identical(
            decimal_scaled_integer<int, -3>{1}/decimal_scaled_integer<int, 0>{3},
            decimal_scaled_integer<int, -3>{0.333}), "");

    static_assert(identical(
            decimal_scaled_integer<int, -3>{2}/decimal_scaled_integer<int, 0>{3},
            decimal_scaled_integer<int, -3>{0.666}), "");
}

namespace test_remainder {
    static_assert(identical(
            decimal_scaled_integer<int, -3>{1}%decimal_scaled_integer<int, 0>{3},
            decimal_scaled_integer<int, -3>{0.001}), "");

    static_assert(identical(
            decimal_scaled_integer<int, -3>{2}%decimal_scaled_integer<int, 0>{3},
            decimal_scaled_integer<int, -3>{0.002}), "");
}
