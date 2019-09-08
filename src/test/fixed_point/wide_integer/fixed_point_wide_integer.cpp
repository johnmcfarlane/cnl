
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/wide_integer.h>
#include <cnl/_impl/num_traits/digits.h>

#include <cinttypes>

#include <gtest/gtest.h>

// TODO: Every `#if !defined(TEST_WIDE_INTEGER)` is a TODO
#define TEST_WIDE_INTEGER
#define TEST_WIDE_INTEGER_INT
#define TEST_LABEL wide_integer_

////////////////////////////////////////////////////////////////////////////////
// wide_integer type used as fixed_point Rep type

using test_int = cnl::wide_integer<cnl::digits<int>::value, int>;

////////////////////////////////////////////////////////////////////////////////
// perform fixed_point tests with this type of fixed_point specialization
#include "../fixed_point_common.h"

TEST(fixed_point_wide_integer, to_string)
{
    auto const expected = "0.714285714285714285714285713708718880752631365184564249393162072010454721748828887939453125";
    auto const actual = cnl::to_string(cnl::fixed_point<cnl::wide_integer<100, unsigned>, -90>{5.}/7);
    ASSERT_EQ(expected, actual);
}

#if !defined(__arm__)
TEST(fixed_point_wide_integer, quotient)
{
    using fixed_point = cnl::fixed_point<cnl::wide_integer<129, unsigned>, -96>;
    auto expected = fixed_point{1.L/3};
    auto actual = fixed_point{cnl::make_fraction(1, 3)};
    ASSERT_EQ(double(expected), double(actual));
}

#if !defined(__GNUC__) || defined(__clang__)
TEST(fixed_point_wide_integer, quotient200)
{
    using fixed_point = cnl::fixed_point<cnl::wide_integer<200, unsigned>, -196>;
    auto expected = fixed_point{5.L/7};
    auto actual = fixed_point{cnl::make_fraction(5, 7)};
    ASSERT_EQ(double(expected), double(actual));
}

TEST(fixed_point_wide_integer, ctor_fraction)
{
    using namespace cnl::literals;
    using fixed_point = cnl::fixed_point<cnl::wide_integer<200>, -100>;

    auto expected = cnl::_impl::from_rep<fixed_point>(cnl::wide_integer<200>{0x5555555555555555555555555_wide});
    auto actual = fixed_point{cnl::fraction<int>(1, 3)};
    ASSERT_EQ(expected, actual);
}
#endif
#endif
