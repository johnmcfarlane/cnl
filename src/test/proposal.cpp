
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
using sg14::set_width_t;
using sg14::sqrt;
using sg14::width;
using std::is_same;
using sg14::_impl::identical;

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

static_assert(identical(fixed_point<uint8_t, -3>{8} + fixed_point<int8_t, -4>{3}, fixed_point<int, -4>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + 3, fixed_point<int, -3>{11}), "Incorrect information in P0037 section, Operator Overloads");
static_assert(identical(fixed_point<uint8_t, -3>{8} + float{3}, float{11}), "Incorrect information in P0037 section, Operator Overloads");

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

// The `width` Helper Type
static_assert(width<int16_t>::value == 16, "Incorrect information in proposal section, The `width` Helper Type");

// The `set_width` and `set_width_t` Helper Types
static_assert(is_same<set_width_t<int8_t, 64>, int64_t>::value, "Incorrect information in proposal section, The `set_width` and `set_width_t` Helper Types");

// Examples
template<class Fp>
constexpr auto magnitude(Fp x, Fp y, Fp z)
-> decltype(sqrt(x*x+y*y+z*z))
{
    return sqrt(x*x+y*y+z*z);
}

TEST(proposal, examples)
{
    auto m = magnitude(
            make_ufixed<4, 12>(1),
            make_ufixed<4, 12>(4),
            make_ufixed<4, 12>(9));
    static_assert(is_same<decltype(m), make_fixed<19, 12>>::value, "Incorrect formation in proposal section, Examples");
    ASSERT_EQ(m, 9.8994140625);
}

TEST(proposal, zero)
{
    static fixed_point<> zero;
    ASSERT_EQ(zero, fixed_point<>(0));
}
