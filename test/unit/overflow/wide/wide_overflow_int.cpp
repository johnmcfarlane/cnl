
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/ostream.h>
#include <cnl/overflow_integer.h>
#include <cnl/wide_integer.h>

#include <test.h>

#include <cmath>

template<int Digits, cnl::overflow_tag OverflowTag = cnl::native_overflow_tag>
using wide_overflow_integer = cnl::overflow_integer<cnl::wide_integer<Digits>, OverflowTag>;

namespace {
    TEST(wide_overflow_integer, divide)  // NOLINT
    {
        auto expected = wide_overflow_integer<100>{std::pow(2.L, 50.L) / 3};
        auto dividend = wide_overflow_integer<100>(1) << 50;
        auto divisor = wide_overflow_integer<100>(3);
        auto actual = dividend / divisor;
        ASSERT_EQ(expected, actual);
    }
}
