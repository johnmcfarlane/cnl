
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

TEST(fixed_point_wide_integer, quotient200)
{
    using fixed_point = cnl::fixed_point<cnl::wide_integer<200, unsigned>, -196>;
    auto expected = fixed_point{5.L/7};
    auto actual = cnl::quotient<fixed_point>(5, 7);
    ASSERT_EQ(double(expected), double(actual));
}

TEST(fixed_point_wide_integer, quotient)
{
    using fixed_point = cnl::fixed_point<cnl::wide_integer<129, unsigned>, -96>;
    auto expected = fixed_point{1.L/3};
    auto actual = cnl::quotient<fixed_point>(1, 3);
    ASSERT_EQ(double(expected), double(actual));
}

TEST(fixed_point_wide_integer, ctor_fraction)
{
    using wide_integer = cnl::wide_integer<200>;
    using fixed_point = cnl::fixed_point<wide_integer, -100>;
    using duplex_integer = wide_integer::rep::rep;
    auto expected = cnl::_impl::from_rep<fixed_point>(wide_integer{duplex_integer{
#if defined(CNL_INT128_ENABLED)
            0, {0x555555555ULL, 0x5555555555555555ULL}
#else
            {{0, 0}, {0, 5}}, {{0x55555555, 0x55555555}, 0x55555555}
#endif
    }});
    auto actual = fixed_point{cnl::fraction<int>(1, 3)};
    ASSERT_EQ(expected, actual);
}
