
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/scaled_integer.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::scaled_integer;
using cnl::_impl::identical;

TEST(utils_tests, sin)
{
    ASSERT_EQ(sin(scaled_integer<cnl::uint8, -6>(0)), 0);
    ASSERT_EQ(sin(scaled_integer<cnl::int16, -13>(3.1415926)), 0.f);
    ASSERT_EQ(sin(scaled_integer<cnl::uint16, -14>(3.1415926/2)), 1);
    ASSERT_EQ(sin(scaled_integer<cnl::int32, -24>(3.1415926*7./2.)), -1);
    ASSERT_EQ(sin(scaled_integer<cnl::int32, -28>(3.1415926/4)), .707106769f);
    ASSERT_EQ(sin(scaled_integer<cnl::int16, -10>(-3.1415926/3)), -.865234375);
}

TEST(utils_tests, cos)
{
    ASSERT_EQ(cos(scaled_integer<cnl::uint8, -6>(0)), 1.f);
    ASSERT_EQ(cos(scaled_integer<cnl::int16, -13>(3.1415926)), -1);
    ASSERT_EQ(cos(scaled_integer<cnl::uint16, -14>(3.1415926/2)), 0.L);
    ASSERT_EQ(cos(scaled_integer<cnl::int32, -20>(3.1415926*7./2.)), 0.f);
    ASSERT_EQ(cos(scaled_integer<cnl::int32, -28>(3.1415926/4)), .707106829f);
    ASSERT_EQ(cos(scaled_integer<cnl::int16, -10>(-3.1415926/3)), .5L);
}

////////////////////////////////////////////////////////////////////////////////
// cnl::abs

static_assert(abs(scaled_integer<cnl::int8, 0>(66))==66, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int8, 0>(-123))==123, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int64, 0>(9223372036854775807))==9223372036854775807LL, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int64, 0>(-9223372036854775807))==9223372036854775807LL, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int16, 0>(-5))==5, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int64, -29>(-5LL))==5LL, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int64, -30>(-5LL))==5LL, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::int64, -31>(-5LL))==5LL, "cnl::abs test failed");

static_assert(abs(scaled_integer<cnl::uint8, 0>(66))==66, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::uint8, 0>(123))==123, "cnl::abs test failed");
static_assert(abs(scaled_integer<cnl::uint16, -8>(5))==5, "cnl::abs test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::sqrt

static_assert(static_cast<float>(sqrt(scaled_integer<cnl::int32, -20>(0))) == 0.0f, "cnl::sqrt<scaled_integer>");
static_assert(static_cast<float>(sqrt(scaled_integer<cnl::int32, -20>(2.0))) > 1.414213f, "cnl::sqrt<scaled_integer>");
static_assert(static_cast<float>(sqrt(scaled_integer<cnl::int32, -20>(2.0))) < 1.414214f, "cnl::sqrt<scaled_integer>");
static_assert(static_cast<float>(sqrt(scaled_integer<cnl::int32, -20>(4.0))) == 2.0f, "cnl::sqrt<scaled_integer>");

////////////////////////////////////////////////////////////////////////////////
// cnl::floor

static_assert(identical(
        cnl::scaled_integer<cnl::int32, 0>(100),
        floor(cnl::scaled_integer<cnl::int32, -20>(100.125))), "cnl::floor(scaled_integer)");
static_assert(identical(
        cnl::scaled_integer<cnl::int32, 0>(-101),
        floor(cnl::scaled_integer<cnl::int32, -20>(-100.125))), "cnl::floor(scaled_integer)");
static_assert(identical(
        cnl::scaled_integer<cnl::int32, 1>(-100),
        floor(cnl::scaled_integer<cnl::int32, 1>(-100))), "cnl::floor(scaled_integer)");

////////////////////////////////////////////////////////////////////////////////
// std specializations for 128-bit integer facilitate certain 64-bit operations

#if defined(CNL_INT128_ENABLED)
static_assert((scaled_integer<cnl::uint64, -8>(1003006L)*scaled_integer<cnl::uint64, -8>(7))==7021042UL, "cnl::scaled_integer test failed");
static_assert(static_cast<int>((scaled_integer<cnl::uint64, -8>(65535)/scaled_integer<cnl::uint64, -8>(256)))==255,
        "cnl::scaled_integer test failed");
static_assert(sqrt(scaled_integer<cnl::uint64, 0>(9223372036854775807))==3037000499ULL, "cnl::sqrt test failed");
#endif
