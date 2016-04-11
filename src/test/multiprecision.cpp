
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <multiprecision.h>

#include <gtest/gtest.h>

using sg14::is_integral;
using sg14::is_signed;
using sg14::is_unsigned;
using sg14::fixed_point;
using sg14::make_signed;
using sg14::make_unsigned;
using sg14::multiprecision;
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

TEST(multiprecision, add)
{
    using int64 = unsigned_multiprecision<64>;

    auto augend = int64{123456789012345678LL};
    auto addend = int64{876543210987654321LL};

    auto sum = augend+addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(multiprecision, subtract)
{
    using int64 = sg14::unsigned_multiprecision<64>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend-subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(multiprecision, multiply)
{
    using int64 = unsigned_multiprecision<64>;
    using int128 = unsigned_multiprecision<128>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128>(factor)*static_cast<int128>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product/factor;
    ASSERT_EQ(factor, quotient);
}

TEST(multiprecision, divide)
{
    using int64 = unsigned_multiprecision<64>;

    auto div = int64{123456789012345678LL};

    auto quotient = div/div;
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}

////////////////////////////////////////////////////////////////////////////////
// sg14::fixed_point<sg14::unsigned_multiprecision<>> arithmetic

TEST(fixed_point_multiprecision, add)
{
    using int64 = fixed_point<unsigned_multiprecision<64>>;

    auto augend = int64{123456789012345678LL};
    auto addend = int64{876543210987654321LL};

    auto sum = augend+addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(fixed_point_multiprecision, subtract)
{
    using int64 = fixed_point<unsigned_multiprecision<64>>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend-subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(fixed_point_multiprecision, multiply)
{
    using int64 = fixed_point<unsigned_multiprecision<64>>;
    using int128 = fixed_point<unsigned_multiprecision<128>>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128>(factor)*static_cast<int128>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product/factor;
    ASSERT_EQ(factor, quotient);
}

TEST(fixed_point_multiprecision, divide)
{
    using int64 = fixed_point<unsigned_multiprecision<64>>;

    auto div = int64{123456789012345678LL};

    auto quotient = div/div;
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}

////////////////////////////////////////////////////////////////////////////////
// sg14::elastic_multiprecision arithmetic

TEST(elastic_multiprecision, add)
{
    using int64 = elastic_multiprecision<64, 0, false>;

    auto augend = int64{123456789012345678LL};
    auto addend = int64{876543210987654321LL};

    auto sum = augend+addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(elastic_multiprecision, subtract)
{
    using int64 = elastic_multiprecision<64, 0, false>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend-subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(elastic_multiprecision, multiply)
{
    using int64 = elastic_multiprecision<64, 0, false>;
    using int128 = elastic_multiprecision<128, 0, false>;

    auto factor = int64{123456789012345678LL};

    auto product = factor*factor;
    static_assert(std::is_same<int128, decltype(product)>::value, "");
    ASSERT_GT(product, factor);

    auto quotient = product/factor;
    ASSERT_EQ(factor, quotient);
}

TEST(elastic_multiprecision, divide)
{
    using int64 = elastic_multiprecision<64, 0, false>;

    auto div = int64{123456789012345678LL};

    auto quotient = div/div;
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}
