
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(CNL_BOOST_ENABLED) && !defined(_WIN64)

#include <cnl/auxiliary/boost.multiprecision.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>
#include <cnl/scaled_integer.h>

#include <boost/version.hpp>
#include <gtest/gtest.h>

#include "boost.throw_exception.h"

using cnl::_impl::assert_same;
using cnl::_impl::identical;
using cnl::scaled_integer;
using cnl::multiprecision;
using cnl::set_digits_t;
using cnl::signed_multiprecision;
using cnl::unsigned_multiprecision;

////////////////////////////////////////////////////////////////////////////////
// cnl::multiprecision traits

namespace test_is_boost_multiprecision {
    static_assert(
            cnl::_impl::is_boost_multiprecision<cnl::unsigned_multiprecision<987654321>>::value,
            "cnl::is_boost_multiprecision<>");
}

// test cnl::is_signed<{un}signed_multiprecision>::value
static_assert(cnl::is_signed<signed_multiprecision<1 >>::value,
        "cnl::is_signed<signed_multiprecision<>>::value test failed");
static_assert(cnl::is_signed<signed_multiprecision<3 >>::value,
        "cnl::is_signed<signed_multiprecision<>>::value test failed");
static_assert(!cnl::is_signed<unsigned_multiprecision<5 >>::value,
        "cnl::is_signed<unsigned_multiprecision<>>::value test failed");
static_assert(!cnl::is_signed<unsigned_multiprecision<7 >>::value,
        "cnl::is_signed<unsigned_multiprecision<>>::value test failed");

// test cnl::add_signedness<{un}signed_multiprecision>::add_signedness
static_assert(cnl::is_signed<cnl::add_signedness_t<signed_multiprecision<9>>>::value,
        "cnl::add_signedness<{un}signed_multiprecision>::add_signedness test failed");
static_assert(cnl::is_signed<cnl::add_signedness_t<unsigned_multiprecision<10 >>>::value,
        "cnl::add_signedness<{un}signed_multiprecision>::add_signedness test failed");
static_assert(cnl::is_signed<cnl::add_signedness_t<signed_multiprecision<9 >>>::value,
        "cnl::add_signedness<{un}signed_multiprecision>::add_signedness test failed");
static_assert(cnl::is_signed<cnl::add_signedness_t<unsigned_multiprecision<10 >>>::value,
        "cnl::add_signedness<{un}signed_multiprecision>::add_signedness test failed");

// test cnl::remove_signedness_t<{un}signed_multiprecision>
static_assert(!cnl::is_signed<cnl::remove_signedness_t<signed_multiprecision<9 >>>::value,
        "cnl::remove_signedness<{un}signed_multiprecision>::remove_signedness test failed");
static_assert(!cnl::is_signed<cnl::remove_signedness_t<unsigned_multiprecision<10 >>>::value,
        "cnl::remove_signedness<{un}signed_multiprecision>::remove_signedness test failed");
static_assert(!cnl::is_signed<cnl::remove_signedness_t<signed_multiprecision<9 >>>::value,
        "cnl::remove_signedness<{un}signed_multiprecision>::remove_signedness test failed");
static_assert(!cnl::is_signed<cnl::remove_signedness_t<unsigned_multiprecision<10 >>>::value,
        "cnl::remove_signedness<{un}signed_multiprecision>::remove_signedness test failed");

// test cnl::width<{un}signed_multiprecision>
static_assert(cnl::digits<signed_multiprecision<11>>::value>=11, "cnl::digits<signed_multiprecision>::value test failed");
static_assert(cnl::digits<unsigned_multiprecision<12>>::value>=12, "cnl::digits<signed_multiprecision>::value test failed");

// test cnl::set_digits_t<{un}signed_multiprecision>
static_assert(cnl::digits<set_digits_t<signed_multiprecision<13>, 57>>::value>=57, "set_digits_t<signed_multiprecision> test failed");
static_assert(cnl::digits<set_digits_t<unsigned_multiprecision<14>, 3>>::value>=3, "set_digits_t<signed_multiprecision> test failed");

static_assert(cnl::digits<set_digits_t<signed_multiprecision<120>, 3>>::value<=cnl::digits<signed_multiprecision<120>>::value, "set_digits_t<signed_multiprecision> test failed");
static_assert(cnl::digits<unsigned_multiprecision<128>>::value>=cnl::digits<set_digits_t<unsigned_multiprecision<128>, 16>>::value, "set_digits_t<signed_multiprecision> test failed");

namespace test_to_rep {
    static_assert(
            assert_same<
                    decltype(std::declval<unsigned_multiprecision<12>>()),
                    decltype(cnl::to_rep<scaled_integer<unsigned_multiprecision<12>>>{}(
                            std::declval<unsigned_multiprecision<12>>()))>::value,
            "cnl::_impl::depth<scaled_integer<boost::multiprecision>>");
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
            assert_same<
                    unsigned_multiprecision<987654321>,
                    cnl::_impl::rep_t<scaled_integer<unsigned_multiprecision<987654321>>>>::value,
            "cnl::_impl::rep_t<scaled_integer<boost::multiprecision>>");
}

TEST(multiprecision, from_value)  // NOLINT
{
    auto const expected = unsigned_multiprecision<43212>{53};
    auto const actual = cnl::from_value<
            signed_multiprecision<47>,
            unsigned_multiprecision<43212>>{}(unsigned_multiprecision<786>{53});
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

TEST(multiprecision, scale_positive)  // NOLINT
{
    auto const expected = signed_multiprecision<321>{768*(1 << 5)};
    auto const actual = cnl::_impl::scale<5, 2>(signed_multiprecision<321>{768});
    ASSERT_EQ(expected, actual);
    ASSERT_TRUE(identical(expected, actual));
}

TEST(multiprecision, scale_negative)  // NOLINT
{
    auto const expected = unsigned_multiprecision<321>{768/(1 << 5)};
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

    auto sum = augend+addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(multiprecision, subtract)  // NOLINT
{
    using int64 = cnl::unsigned_multiprecision<64>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend-subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(multiprecision, multiply)  // NOLINT
{
    using int64 = unsigned_multiprecision<64>;
    using int128 = unsigned_multiprecision<128>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128>(factor)*static_cast<int128>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product/factor;
    ASSERT_EQ(factor, quotient);
}

TEST(multiprecision, divide)  // NOLINT
{
    using int64 = unsigned_multiprecision<64>;

    auto div = int64{123456789012345678LL};

    auto quotient = div/div;  // NOLINT(misc-redundant-expression)
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

    auto sum = augend+addend;
    auto expected = int64{999999999999999999LL};

    ASSERT_EQ(sum, expected);
}

TEST(scaled_integer_multiprecision, subtract)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;

    auto minuend = int64{999999999999999999LL};
    auto subtrahend = int64{876543210987654321LL};

    auto difference = minuend-subtrahend;
    auto expected = int64{123456789012345678LL};

    ASSERT_EQ(difference, expected);
}

TEST(scaled_integer_multiprecision, multiply)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;
    using int128 = scaled_integer<unsigned_multiprecision<128>>;

    auto factor = int64{123456789012345678LL};

    auto product = static_cast<int128>(factor)*static_cast<int128>(factor);
    ASSERT_GT(product, factor);

    auto quotient = product/factor;
    ASSERT_EQ(factor, quotient);
}

TEST(scaled_integer_multiprecision, divide)  // NOLINT
{
    using int64 = scaled_integer<unsigned_multiprecision<64>>;

    auto div = int64{123456789012345678LL};

    auto quotient = div/div;  // NOLINT(misc-redundant-expression)
    auto expected = 1;

    ASSERT_EQ(quotient, expected);
}

////////////////////////////////////////////////////////////////////////////////
// cnl::digits<boost::multiprecision::number<>>

static_assert(
        cnl::digits<unsigned_multiprecision<24>>::value == 24,
        "cnl::digits<boost::multiprecision::number<>> test failed");

static_assert(
        cnl::digits<signed_multiprecision<24>>::value == 24,
        "cnl::digits<boost::multiprecision::number<>> test failed");

////////////////////////////////////////////////////////////////////////////////
// bitwise shift with boost::multiprecision and cnl::constant

// Boost.Multiprecision in 1.58 known to have constexpr support
#if (BOOST_VERSION>=105800)

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
    using length_t = cnl::scaled_integer<boost::multiprecision::int128_t> ;
    length_t x = length_t{ 25 };
    auto y = cnl::sqrt(x);
    ASSERT_EQ(y, 5);
}

#endif  // CNL_BOOST_ENABLED
