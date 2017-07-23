
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/bits/type_traits.h>
#include <cnl/fixed_point>

#include <gtest/gtest.h>

using cnl::fixed_point;
using cnl::make_fixed;
using cnl::make_ufixed;

TEST(utils_tests, sin)
{
    ASSERT_EQ(sin(fixed_point<std::uint8_t, -6>(0)), 0);
    ASSERT_EQ(sin(fixed_point<std::int16_t, -13>(3.1415926)), 0.f);
    ASSERT_EQ(sin(fixed_point<std::uint16_t, -14>(3.1415926/2)), 1);
    ASSERT_EQ(sin(fixed_point<std::int32_t, -24>(3.1415926*7./2.)), -1);
    ASSERT_EQ(sin(fixed_point<std::int32_t, -28>(3.1415926/4)), .707106769f);
    ASSERT_EQ(sin(fixed_point<std::int16_t, -10>(-3.1415926/3)), -.865234375);
}

TEST(utils_tests, cos)
{
    ASSERT_EQ(cos(fixed_point<std::uint8_t, -6>(0)), 1.f);
    ASSERT_EQ(cos(fixed_point<std::int16_t, -13>(3.1415926)), -1);
    ASSERT_EQ(cos(fixed_point<std::uint16_t, -14>(3.1415926/2)), 0.L);
    ASSERT_EQ(cos(fixed_point<std::int32_t, -20>(3.1415926*7./2.)), 0.f);
    ASSERT_EQ(cos(fixed_point<std::int32_t, -28>(3.1415926/4)), .707106829f);
    ASSERT_EQ(cos(fixed_point<std::int16_t, -10>(-3.1415926/3)), .5L);
}

////////////////////////////////////////////////////////////////////////////////
// cnl::abs

static_assert(abs(make_fixed<7, 0>(66))==66, "cnl::abs test failed");
static_assert(abs(make_fixed<7, 0>(-123))==123, "cnl::abs test failed");
static_assert(abs(make_fixed<63, 0>(9223372036854775807))==9223372036854775807LL, "cnl::abs test failed");
static_assert(abs(make_fixed<63, 0>(-9223372036854775807))==9223372036854775807LL, "cnl::abs test failed");
static_assert(abs(make_fixed<7, 8>(-5))==5, "cnl::abs test failed");

static_assert(abs(make_ufixed<8, 0>(66))==66, "cnl::abs test failed");
static_assert(abs(make_ufixed<8, 0>(123))==123, "cnl::abs test failed");
static_assert(abs(make_ufixed<8, 8>(5))==5, "cnl::abs test failed");

////////////////////////////////////////////////////////////////////////////////
// std specializations for 128-bit integer facilitate certain 64-bit operations

#if defined(SG14_INT128_ENABLE)
static_assert((make_ufixed<56, 8>(1003006)*make_ufixed<56, 8>(7))==7021042, "cnl::fixed_point test failed");
static_assert(static_cast<int>((fixed_point<uint64_t, -8>(65535)/fixed_point<uint64_t, -8>(256)))==255,
        "cnl::fixed_point test failed");
static_assert(sqrt(make_fixed<63, 0>(9223372036854775807))==3037000499LL, "cnl::sqrt test failed");
#endif
