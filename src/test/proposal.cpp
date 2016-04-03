
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sample_functions.h"

#include <gtest/gtest.h>

using sg14::fixed_point;
using sg14::make_fixed;
using sg14::make_ufixed;
using sg14::multiply;
using std::is_same;

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in P0037

TEST(proposal, make_ufixed)
{
    make_ufixed<4, 4> value{15.9375};
    ASSERT_EQ(value, 15.9375);
}

TEST(proposal, make_fixed)
{
    make_fixed<2, 29> value{3.141592653};
    ASSERT_EQ(value, 3.1415926516056061);
}

// Conversion
static_assert(make_ufixed<4, 4>{.006}==make_ufixed<4, 4>{0}, "Incorrect information in proposal section, Conversion");

// Operator Overloads
static_assert(fixed_point<uint8_t, -3>{8} + fixed_point<uint8_t, -4>{3} == fixed_point<unsigned, -3>{11},
        "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(fixed_point<uint8_t, -3>{8} + fixed_point<int8_t, -4>{3}), decltype(fixed_point<int, -3>{11})>::value,
        "Incorrect information in proposal section, Operator Overloads");

static_assert(make_ufixed<5, 3>{8}+3==fixed_point<signed, -3>{11},
        "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(make_ufixed<5, 3>{8}+3), decltype(fixed_point<signed, -3>{11})>::value,
        "Incorrect information in proposal section, Operator Overloads");

static_assert(make_ufixed<5, 3>{8}+float{3}==float{11},
        "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(make_ufixed<5, 3>{8}+float{3}), decltype(float{11})>::value,
        "Incorrect information in proposal section, Operator Overloads");

// Overflow
TEST(proposal, overflow) {
    switch (sizeof(int)) {
    case 4: {
        auto sum = make_ufixed<2, 30>(3) + make_ufixed<2, 30>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    case 8: {
        auto sum = make_ufixed<2, 62>(3) + make_ufixed<2, 62>(1);
        ASSERT_TRUE(sum == 0);
        break;
    }
    default:
        FAIL() << "dying to know what architecture this is";
    }
}

// Underflow
static_assert(make_fixed<7, 0>(15)/make_fixed<7, 0>(2)==7.f, "Incorrect information in proposal section, Underflow");

// Named Arithmetic Functions
TEST(proposal, named_arithmetic1)
{
    auto f = make_ufixed<4, 4>{15.9375};
    auto p = multiply<make_ufixed<8, 8>>(f, f);

    static_assert(is_same<decltype(p), make_ufixed<8, 8>>::value, "Incorrect formation in proposal section, Named Arithmetic Functions");
    ASSERT_EQ(p, 254.00390625);
}

TEST(proposal, named_arithmetic2)
{
    auto f = make_ufixed<4, 4>{15.9375};
    auto p = multiply<make_ufixed<4, 4>>(f, f);

    static_assert(is_same<decltype(p), make_ufixed<4, 4>>::value, "Incorrect formation in proposal section, Named Arithmetic Functions");
    ASSERT_EQ(p, 14.00000000);
}

TEST(proposal, named_arithmetic3)
{
    auto f = make_ufixed<4, 4>{15.9375};
    auto p = f * f;

    static_assert(is_same<decltype(p), make_fixed<27, 4>>::value, "Incorrect formation in proposal section, Named Arithmetic Functions");
    ASSERT_EQ(p, 254.00000000);
}

TEST(proposal, zero)
{
    static fixed_point<> zero;
    ASSERT_EQ(zero, fixed_point<>(0));
}
