
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <multiprecision.h>

#include <gtest/gtest.h>

using sg14::is_integral;
using sg14::is_signed;
using sg14::is_unsigned;
using sg14::make_signed;
using sg14::make_unsigned;
using sg14::elastic_multiprecision;
using sg14::elasticate;
using sg14::resize;
using sg14::signed_multiprecision;
using sg14::unsigned_multiprecision;
using sg14::width;

////////////////////////////////////////////////////////////////////////////////
// sg14::multiprecision traits

// test sg14::is_signed<{un}signed_multiprecision>
static_assert(is_signed<signed_multiprecision<1 >>::value, "sg14::is_signed<signed_multiprecision<>> test failed");
static_assert(is_signed<signed_multiprecision<3 >>::value, "sg14::is_signed<signed_multiprecision<>> test failed");
static_assert(
        !is_signed<unsigned_multiprecision<5 >>::value, "sg14::is_signed<unsigned_multiprecision<>> test failed");
static_assert(
        !is_signed<unsigned_multiprecision<7 >>::value, "sg14::is_signed<unsigned_multiprecision<>> test failed");

// test sg14::is_unsigned<{un}signed_multiprecision>
static_assert(
        !is_unsigned<signed_multiprecision<2 >>::value, "sg14::is_unsigned<signed_multiprecision<>> test failed");
static_assert(
        !is_unsigned<signed_multiprecision<4 >>::value, "sg14::is_unsigned<signed_multiprecision<>> test failed");
static_assert(
        is_unsigned<unsigned_multiprecision<6 >>::value, "sg14::is_unsigned<unsigned_multiprecision<>> test failed");
static_assert(
        is_unsigned<unsigned_multiprecision<8 >>::value, "sg14::is_unsigned<unsigned_multiprecision<>> test failed");

// test sg14::make_signed<{un}signed_multiprecision>
static_assert(is_signed<make_signed<signed_multiprecision<9 >>::type>::value,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(is_signed<make_signed<unsigned_multiprecision<10 >>::type>::value,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(!is_unsigned<make_signed<signed_multiprecision<9 >>::type>::value,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(!is_unsigned<make_signed<unsigned_multiprecision<10 >>::type>::value,
        "sg14::make_signed<signed_multiprecision<>> test failed");

// test sg14::make_unsigned<{un}signed_multiprecision>
static_assert(!is_signed<make_unsigned<signed_multiprecision<9 >>::type>::value,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(!is_signed<make_unsigned<unsigned_multiprecision<10 >>::type>::value,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(is_unsigned<make_unsigned<signed_multiprecision<9 >>::type>::value,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(is_unsigned<make_unsigned<unsigned_multiprecision<10 >>::type>::value,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");

// test sg14::resize<{un}signed_multiprecision>
static_assert(width<signed_multiprecision<11>>::value>=11, "width<signed_multiprecision> test failed");
static_assert(width<unsigned_multiprecision<12>>::value>=12, "width<signed_multiprecision> test failed");

static_assert(
        width<
                resize<
                        signed_multiprecision<13>,
                        57
                >::type
        >::value>=57, "width<signed_multiprecision> test failed");
static_assert(width<resize<unsigned_multiprecision<14>, 3>::type>::value>=3, "width<unsigned_multiprecision> test failed");

static_assert(width<resize<signed_multiprecision<120>, 3>::type>::value<=width<signed_multiprecision<120>>::value,
        "width<signed_multiprecision> test failed");
static_assert(width<unsigned_multiprecision<128>>::value>=width<resize<unsigned_multiprecision<128>, 16>::type>::value,
        "width<signed_multiprecision>::value test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::multiprecision arithmetic

TEST(multiprecision, divide) {
    auto n = elastic_multiprecision<64, 0, false>{123456789012345678};
    ASSERT_EQ(n / n, 1);
}
