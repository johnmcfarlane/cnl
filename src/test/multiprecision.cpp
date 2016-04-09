
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

////////////////////////////////////////////////////////////////////////////////
// traits

// test sg14::is_signed<{un}signed_multiprecision>
static_assert(is_signed<signed_multiprecision<1>>::value, "sg14::is_signed<signed_multiprecision<>> test failed");
static_assert(is_signed<signed_multiprecision<3>>::value, "sg14::is_signed<signed_multiprecision<>> test failed");
static_assert(
        !is_signed<unsigned_multiprecision<5>>::value, "sg14::is_signed<unsigned_multiprecision<>> test failed");
static_assert(
        !is_signed<unsigned_multiprecision<7>>::value, "sg14::is_signed<unsigned_multiprecision<>> test failed");

// test sg14::is_unsigned<{un}signed_multiprecision>
static_assert(
        !is_unsigned<signed_multiprecision<2>>::value, "sg14::is_unsigned<signed_multiprecision<>> test failed");
static_assert(
        !is_unsigned<signed_multiprecision<4>>::value, "sg14::is_unsigned<signed_multiprecision<>> test failed");
static_assert(
        is_unsigned<unsigned_multiprecision<6>>::value, "sg14::is_unsigned<unsigned_multiprecision<>> test failed");
static_assert(
        is_unsigned<unsigned_multiprecision<8>>::value, "sg14::is_unsigned<unsigned_multiprecision<>> test failed");

// test sg14::make_signed<{un}signed_multiprecision>
static_assert(is_signed<make_signed<signed_multiprecision<9>>::type>::value, "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(is_signed<make_signed<unsigned_multiprecision<10>>::type>::value, "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(!is_unsigned<make_signed<signed_multiprecision<9>>::type>::value, "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(!is_unsigned<make_signed<unsigned_multiprecision<10>>::type>::value, "sg14::make_signed<signed_multiprecision<>> test failed");

// test sg14::make_unsigned<{un}signed_multiprecision>
static_assert(!is_signed<make_unsigned<signed_multiprecision<9>>::type>::value, "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(!is_signed<make_unsigned<unsigned_multiprecision<10>>::type>::value, "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(is_unsigned<make_unsigned<signed_multiprecision<9>>::type>::value, "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(is_unsigned<make_unsigned<unsigned_multiprecision<10>>::type>::value, "sg14::make_unsigned<signed_multiprecision<>> test failed");

// test sg14::resize<{un}signed_multiprecision>
static_assert(sizeof(signed_multiprecision<11>) >= 11, "sizeof(signed_multiprecision) test failed");
static_assert(sizeof(unsigned_multiprecision<12>) >= 12, "sizeof(signed_multiprecision) test failed");

static_assert(sizeof(resize<signed_multiprecision<13>, 57>::type) >= 57, "sizeof(signed_multiprecision) test failed");
static_assert(sizeof(resize<unsigned_multiprecision<14>, 3>::type) >= 3, "sizeof(unsigned_multiprecision) test failed");

static_assert(sizeof(resize<signed_multiprecision<15>, 3>::type) <= sizeof(signed_multiprecision<15>), "sizeof(signed_multiprecision) test failed");
static_assert(sizeof(unsigned_multiprecision<16>) >= sizeof(resize<unsigned_multiprecision<16>, 16>::type), "sizeof(signed_multiprecision) test failed");

////////////////////////////////////////////////////////////////////////////////
// elastic_multiprecision

TEST(multiprecision, divide) {
    auto n = elastic_multiprecision<64, 0, false>{123456789012345678};
    ASSERT_EQ(n / n, 1);
}
