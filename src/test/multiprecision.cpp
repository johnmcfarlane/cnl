
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(SG14_BOOST_ENABLED)

#include <sg14/auxiliary/multiprecision.h>

#include <sg14/fixed_point>

#include <gtest/gtest.h>

using sg14::fixed_point;
using sg14::make_signed;
using sg14::make_unsigned;
using sg14::multiprecision;
using sg14::set_width_t;
using sg14::signed_multiprecision;
using sg14::unsigned_multiprecision;
using sg14::width;

////////////////////////////////////////////////////////////////////////////////
// sg14::multiprecision traits

// test std::numeric_limits<{un}signed_multiprecision>::is_signed
static_assert(std::numeric_limits<signed_multiprecision<1 >>::is_signed,
        "std::numeric_limits<signed_multiprecision<>>::is_signed test failed");
static_assert(std::numeric_limits<signed_multiprecision<3 >>::is_signed,
        "std::numeric_limits<signed_multiprecision<>>::is_signed test failed");
static_assert(!std::numeric_limits<unsigned_multiprecision<5 >>::is_signed,
        "std::numeric_limits<unsigned_multiprecision<>>::is_signed test failed");
static_assert(!std::numeric_limits<unsigned_multiprecision<7 >>::is_signed,
        "std::numeric_limits<unsigned_multiprecision<>>::is_signed test failed");

// test sg14::make_signed<{un}signed_multiprecision>
static_assert(std::numeric_limits<make_signed<signed_multiprecision<9 >>::type>::is_signed,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(std::numeric_limits<make_signed<unsigned_multiprecision<10 >>::type>::is_signed,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(std::numeric_limits<make_signed<signed_multiprecision<9 >>::type>::is_signed,
        "sg14::make_signed<signed_multiprecision<>> test failed");
static_assert(std::numeric_limits<make_signed<unsigned_multiprecision<10 >>::type>::is_signed,
        "sg14::make_signed<signed_multiprecision<>> test failed");

// test sg14::make_unsigned<{un}signed_multiprecision>
static_assert(!std::numeric_limits<make_unsigned<signed_multiprecision<9 >>::type>::is_signed,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(!std::numeric_limits<make_unsigned<unsigned_multiprecision<10 >>::type>::is_signed,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(!std::numeric_limits<make_unsigned<signed_multiprecision<9 >>::type>::is_signed,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");
static_assert(!std::numeric_limits<make_unsigned<unsigned_multiprecision<10 >>::type>::is_signed,
        "sg14::make_unsigned<signed_multiprecision<>> test failed");

// test sg14::width<{un}signed_multiprecision>
static_assert(width<signed_multiprecision<11>>::value>=11, "width<signed_multiprecision> test failed");
static_assert(width<unsigned_multiprecision<12>>::value>=12, "width<signed_multiprecision> test failed");

// test sg14::set_width_t<{un}signed_multiprecision>
static_assert(width<set_width_t<signed_multiprecision<13>, 57>>::value>=57, "width<signed_multiprecision> test failed");
static_assert(width<set_width_t<unsigned_multiprecision<14>, 3>>::value>=3, "width<unsigned_multiprecision> test failed");

static_assert(width<set_width_t<signed_multiprecision<120>, 3>>::value<=width<signed_multiprecision<120>>::value, "width<signed_multiprecision> test failed");
static_assert(width<unsigned_multiprecision<128>>::value>=width<set_width_t<unsigned_multiprecision<128>, 16>>::value, "width<signed_multiprecision>::value test failed");

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
// boost::throw_exception

#if defined(BOOST_NO_EXCEPTIONS)
namespace boost {
    void throw_exception(std::exception const &) {
        std::terminate();
    }
}
#endif

#endif  // SG14_BOOST_ENABLED
