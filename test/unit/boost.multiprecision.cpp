
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/auxiliary/boost.multiprecision.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/scaled_integer.h>

#include <boost/version.hpp>  // NOLINT(llvm-include-order)
#include <gtest/gtest.h>

#include "boost.throw_exception.h"

#include <type_traits>

using cnl::multiprecision;
using cnl::scaled_integer;
using cnl::set_digits_t;
using cnl::signed_multiprecision;
using cnl::unsigned_multiprecision;
using cnl::_impl::identical;

////////////////////////////////////////////////////////////////////////////////
// cnl::multiprecision traits

namespace test_is_boost_multiprecision {
    static_assert(
            cnl::_impl::is_boost_multiprecision<cnl::unsigned_multiprecision<987654321>>::value,
            "cnl::is_boost_multiprecision<>");
}

// test cnl::numbers::signedness<{un}signed_multiprecision>::value
static_assert(
        cnl::numbers::signedness<signed_multiprecision<1>>::value,
        "cnl::numbers::signedness<signed_multiprecision<>>::value test failed");
static_assert(
        cnl::numbers::signedness<signed_multiprecision<3>>::value,
        "cnl::numbers::signedness<signed_multiprecision<>>::value test failed");
static_assert(
        !cnl::numbers::signedness<unsigned_multiprecision<5>>::value,
        "cnl::numbers::signedness<unsigned_multiprecision<>>::value test failed");
static_assert(
        !cnl::numbers::signedness<unsigned_multiprecision<7>>::value,
        "cnl::numbers::signedness<unsigned_multiprecision<>>::value test failed");

// test cnl::numbers::set_signedness_t<{un}signed_multiprecision>
static_assert(
        cnl::numbers::signedness<cnl::numbers::set_signedness_t<signed_multiprecision<9>, true>>::value);
static_assert(
        cnl::numbers::signedness<cnl::numbers::set_signedness_t<unsigned_multiprecision<10>, true>>::value);
static_assert(
        cnl::numbers::signedness<cnl::numbers::set_signedness_t<signed_multiprecision<9>, true>>::value);
static_assert(
        cnl::numbers::signedness<cnl::numbers::set_signedness_t<unsigned_multiprecision<10>, true>>::value);

static_assert(
        !cnl::numbers::signedness<cnl::numbers::set_signedness_t<signed_multiprecision<9>, false>>::value);
static_assert(
        !cnl::numbers::signedness<cnl::numbers::set_signedness_t<unsigned_multiprecision<10>, false>>::value);
static_assert(
        !cnl::numbers::signedness<cnl::numbers::set_signedness_t<signed_multiprecision<9>, false>>::value);
static_assert(
        !cnl::numbers::signedness<cnl::numbers::set_signedness_t<unsigned_multiprecision<10>, false>>::value);

// test cnl::width<{un}signed_multiprecision>
static_assert(cnl::digits_v<signed_multiprecision<11>> >= 11);
static_assert(cnl::digits_v<unsigned_multiprecision<12>> >= 12);

// test cnl::set_digits_t<{un}signed_multiprecision>
static_assert(cnl::digits_v<set_digits_t<signed_multiprecision<13>, 57>> >= 57);
static_assert(cnl::digits_v<set_digits_t<unsigned_multiprecision<14>, 3>> >= 3);

static_assert(cnl::digits_v<set_digits_t<signed_multiprecision<120>, 3>> <= cnl::digits_v<signed_multiprecision<120>>);
static_assert(cnl::digits_v<unsigned_multiprecision<128>> >= cnl::digits_v<set_digits_t<unsigned_multiprecision<128>, 16>>);

namespace test_to_rep {
    static_assert(
            std::is_same_v<
                    decltype(std::declval<unsigned_multiprecision<12>>()),
                    decltype(cnl::to_rep<scaled_integer<unsigned_multiprecision<12>>>{}(
                            std::declval<unsigned_multiprecision<12>>()))>);
}

TEST(multiprecision, to_rep)  // NOLINT
{
    auto const expected = unsigned_multiprecision<5432>{123};
    auto const actual = cnl::to_rep<unsigned_multiprecision<5432>>{}(123);
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

namespace test_impl_to_rep {
    static_assert(
            std::is_same_v<
                    unsigned_multiprecision<987654321>,
                    cnl::_impl::rep_of_t<scaled_integer<unsigned_multiprecision<987654321>>>>);
}

TEST(multiprecision, from_value)  // NOLINT
{
    auto const expected = unsigned_multiprecision<43212>{53};
    auto const actual =
            cnl::from_value<signed_multiprecision<47>, unsigned_multiprecision<43212>>{}(
                    unsigned_multiprecision<786>{53});
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

TEST(multiprecision, scale_positive)  // NOLINT
{
    auto const expected = signed_multiprecision<321>{768 * (1 << 5)};
    auto const actual = cnl::_impl::scale<5, 2>(signed_multiprecision<321>{768});
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

TEST(multiprecision, scale_negative)  // NOLINT
{
    auto const expected = unsigned_multiprecision<321>{768 / (1 << 5)};
    auto const actual = cnl::_impl::scale<-5, 2>(unsigned_multiprecision<321>{768});
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

////////////////////////////////////////////////////////////////////////////////
// cnl::multiprecision arithmetic

TEST(multiprecision, add)  // NOLINT
{
    using int64 = unsigned_multiprecision<64>;

    auto augend = int64{123456789012345678LL};
    auto addend = int64{876543210987654321LL};

    auto sum = augend + addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(multiprecision, subtract)  // NOLINT
{
    using int64 = cnl::unsigned_multiprecision<64>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend - subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(multiprecision, multiply)  // NOLINT
{
    using int64 = unsigned_multiprecision<64>;
    using int128_t = unsigned_multiprecision<128>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128_t>(factor) * static_cast<int128_t>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product / factor;
    ASSERT_EQ(factor, quotient);
}

TEST(multiprecision, divide)  // NOLINT
{
    using int64 = unsigned_multiprecision<64>;

    auto div = int64{123456789012345678LL};

    auto quotient = div / div;  // NOLINT(misc-redundant-expression)
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}

////////////////////////////////////////////////////////////////////////////////
// cnl::scaled_integer<cnl::unsigned_multiprecision<>> arithmetic

TEST(scaled_integer_multiprecision, add)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;

    auto augend = int64{123456789012345678LL};
    auto addend = int64{876543210987654321LL};

    auto sum = augend + addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(scaled_integer_multiprecision, subtract)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend - subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(scaled_integer_multiprecision, multiply)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;
    using int128_t = scaled_integer<unsigned_multiprecision<128>>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128_t>(factor) * static_cast<int128_t>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product / factor;
    ASSERT_EQ(factor, quotient);
}

TEST(scaled_integer_multiprecision, divide)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;

    auto div = int64{123456789012345678LL};

    auto quotient = div / div;  // NOLINT(misc-redundant-expression)
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}

////////////////////////////////////////////////////////////////////////////////
// cnl::digits_v<boost::multiprecision::number<>>

static_assert(cnl::digits_v<unsigned_multiprecision<24>> == 24);

static_assert(cnl::digits_v<signed_multiprecision<24>> == 24);

////////////////////////////////////////////////////////////////////////////////
// bitwise shift with boost::multiprecision and cnl::constant

// Boost.Multiprecision in 1.58 known to have constexpr support
#if (BOOST_VERSION >= 105800)

TEST(scaled_integer_multiprecision, shift_left_constant)  // NOLINT
{
    auto lhs = cnl::unsigned_multiprecision<15>{1536};
    auto rhs = cnl::constant<3>{};
    auto actual = lhs << rhs;
    auto expected = cnl::unsigned_multiprecision<15>{1536 << 3};
    ASSERT_EQ(expected, actual);
}

TEST(scaled_integer_multiprecision, shift_right_constant)  // NOLINT
{
    auto lhs = cnl::unsigned_multiprecision<15>{1536};
    auto rhs = cnl::constant<3>{};
    auto actual = lhs >> rhs;
    auto expected = cnl::unsigned_multiprecision<15>{1536 >> 3};
    ASSERT_EQ(expected, actual);
}

#endif

////////////////////////////////////////////////////////////////////////////////
// cnl::used_digits<boost::multiprecision::number<>>

TEST(scaled_integer_multiprecision, unsigned_multiprecision_used_digits)  // NOLINT
{
    using q4_20 = scaled_integer<unsigned_multiprecision<24>, cnl::power<-20>>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(6, cnl::used_digits(a));
}

TEST(scaled_integer_multiprecision, signed_multiprecision_used_digits)  // NOLINT
{
    using q4_20 = scaled_integer<signed_multiprecision<24>, cnl::power<-20>>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(6, cnl::used_digits(a));
}

////////////////////////////////////////////////////////////////////////////////
// cnl::leading_bits<boost::multiprecision::number<>>

TEST(scaled_integer_multiprecision, unsigned_multiprecision_leading_bits)  // NOLINT
{
    using q4_20 = scaled_integer<unsigned_multiprecision<24>, cnl::power<-20>>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(18, leading_bits(a));
}

TEST(scaled_integer_multiprecision, signed_multiprecision_leading_bits)  // NOLINT
{
    using q4_20 = scaled_integer<signed_multiprecision<24>, cnl::power<-20>>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(18, leading_bits(a));
}

TEST(scaled_integer_multiprecision, sqrt)  // NOLINT
{
    using length_t = cnl::scaled_integer<boost::multiprecision::int128_t>;
    length_t x = length_t{25};
    auto y = cnl::sqrt(x);
    ASSERT_EQ(y, 5);
}
