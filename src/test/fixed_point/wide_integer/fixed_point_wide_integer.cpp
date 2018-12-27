
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/wide_integer.h>
#include <cnl/_impl/num_traits/digits.h>

#include <gtest/gtest.h>

#include <cinttypes>

// TODO: Every `#if !defined(TEST_WIDE_INTEGER)` is a TODO
#define TEST_WIDE_INTEGER
#define TEST_WIDE_INTEGER_INT
#define TEST_LABEL wide_integer_

////////////////////////////////////////////////////////////////////////////////
// wide_integer type used as fixed_point Rep type

using test_int = cnl::wide_integer<cnl::digits<int>::value, int>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization

//#include "../fixed_point_common.h"

//TEST(fixed_point_wide_integer, ctor_fraction)
//{
//    constexpr auto s = 10;
//    constexpr auto w = 200;
//    auto const b = std::pow(2.L, s);
//    auto const expected = cnl::wide_integer<w>{b/3};
//    auto numerator = cnl::wide_integer<w>{1};
//    auto denominator = 3;
//    auto actual = numerator / denominator;
//    auto difference = actual - expected;
//    auto diffd = static_cast<double>(difference);
//    ASSERT_LT(diffd, 2.5e-12);
//}

TEST(fixed_point_wide_integer, ctor_fraction)
{
    using fixed_point = cnl::fixed_point<cnl::wide_integer<200>, -100>;
    auto expected = fixed_point{1.L/3};
    auto actual = fixed_point{cnl::fraction<int>(1, 3)};
    auto difference = expected - actual;
    auto diffd = static_cast<double>(difference);
    ASSERT_LT(diffd, 2.5e-12);
}
