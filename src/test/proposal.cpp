
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fixed_point_utils.h>

#include "sample_functions.h"

#include <gtest/gtest.h>

using namespace std;
using namespace sg14;

////////////////////////////////////////////////////////////////////////////////
// Tests of Examples in docs/papers/p0037.html

TEST(proposal, make_ufixed)
{
    make_ufixed<4, 4> value { 15.9375 };
    ASSERT_EQ(value, 15.9375);
}

TEST(proposal, make_fixed)
{
    make_fixed<2, 29> value { 3.141592653 };
    ASSERT_EQ(value, 3.1415926516056061);
}

// Conversion
static_assert(make_ufixed<4, 4>{.006} == make_ufixed<4, 4>{0}, "Incorrect information in proposal section, Conversion");

// Operator Overloads
static_assert(make_ufixed<5, 3>{8} + make_ufixed<4, 4>{3} == make_ufixed<5, 3>{11}, "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(make_ufixed<5, 3>{8} + make_ufixed<4, 4>{3}), decltype(make_ufixed<5, 3>{11})>::value, "Incorrect information in proposal section, Operator Overloads");

static_assert(make_ufixed<5, 3>{8} + 3 == make_ufixed<5, 3>{11}, "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(make_ufixed<5, 3>{8} + 3), decltype(make_ufixed<5, 3>{11})>::value, "Incorrect information in proposal section, Operator Overloads");

static_assert(make_ufixed<5, 3>{8} + float{3} == float{11}, "Incorrect information in proposal section, Operator Overloads");
static_assert(is_same<decltype(make_ufixed<5, 3>{8} + float{3}), decltype(float{11})>::value, "Incorrect information in proposal section, Operator Overloads");

// Overflow
static_assert(static_cast<int>(make_fixed<4, 3>(15) + make_fixed<4, 3>(1)) != 16, "Incorrect information in proposal section, Overflow");

// Underflow
static_assert(make_fixed<7, 0>(15) / make_fixed<7, 0>(2) == 7.f, "Incorrect information in proposal section, Underflow");

TEST(proposal, type_promotion)
{
    auto unpromoted_type = make_fixed<5, 2>(15.5);
    auto type_promotion = promote(unpromoted_type);
    static_assert(is_same<decltype(type_promotion), make_fixed<11, 4>>::value, "Incorrect information in proposal section, Type Promotion");
    ASSERT_EQ(type_promotion, 15.5f);

    auto type_demotion = demote(type_promotion);
    static_assert(is_same<decltype(type_demotion), decltype(unpromoted_type)>::value, "Incorrect information in proposal section, Type Promotion");
    ASSERT_EQ(type_demotion, 15.5f);
}

// Examples
static_assert(magnitude_trunc(
        make_ufixed<4, 12>(1),
        make_ufixed<4, 12>(4),
        make_ufixed<4, 12>(9)) == 9.890625, "unexpected result from magnitude");

TEST(proposal, zero)
{
    static fixed_point<> zero;
    ASSERT_EQ(zero, fixed_point<>(0));
}

TEST(proposal, bounded_integers)
{
	make_ufixed<2, 6> three(3);
	auto n = trunc_square(trunc_square(three));
	ASSERT_EQ(n, 81);
	static_assert(is_same<decltype(n), make_ufixed<8, 0>>::value, "bad assumption about type in 'Bounded Integers' section");
	auto eighty_one = make_ufixed<7, 1>(81);
	ASSERT_EQ(eighty_one, 81);
}
