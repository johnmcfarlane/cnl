
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(CNL_BOOST_ENABLED)

#include <cnl/auxiliary/boost.multiprecision.h>

#include <cnl/fixed_point.h>

#include <boost/version.hpp>
#include <gtest/gtest.h>

#include "boost.throw_exception.h"

using cnl::_impl::assert_same;
using cnl::_impl::identical;
using cnl::fixed_point;
using cnl::multiprecision;
using cnl::set_digits_t;
using cnl::signed_multiprecision;
using cnl::unsigned_multiprecision;

////////////////////////////////////////////////////////////////////////////////
// cnl::multiprecision traits

// test cnl::is_signed<{un}signed_multiprecision>::value
static_assert(cnl::is_signed<signed_multiprecision<1 >>::value,
        "cnl::is_signed<signed_multiprecision<>>::value test failed");
static_assert(cnl::is_signed<signed_multiprecision<3 >>::value,
        "cnl::is_signed<signed_multiprecision<>>::value test failed");
static_assert(!cnl::is_signed<unsigned_multiprecision<5 >>::value,
        "cnl::is_signed<unsigned_multiprecision<>>::value test failed");
static_assert(!cnl::is_signed<unsigned_multiprecision<7 >>::value,
        "cnl::is_signed<unsigned_multiprecision<>>::value test failed");

// test cnl::make_signed<{un}signed_multiprecision>::make_signed
static_assert(cnl::is_signed<cnl::make_signed_t<signed_multiprecision<9>>>::value,
        "cnl::make_signed<{un}signed_multiprecision>::make_signed test failed");
static_assert(cnl::is_signed<cnl::make_signed_t<unsigned_multiprecision<10 >>>::value,
        "cnl::make_signed<{un}signed_multiprecision>::make_signed test failed");
static_assert(cnl::is_signed<cnl::make_signed_t<signed_multiprecision<9 >>>::value,
        "cnl::make_signed<{un}signed_multiprecision>::make_signed test failed");
static_assert(cnl::is_signed<cnl::make_signed_t<unsigned_multiprecision<10 >>>::value,
        "cnl::make_signed<{un}signed_multiprecision>::make_signed test failed");

// test cnl::make_unsigned_t<{un}signed_multiprecision>
static_assert(!cnl::is_signed<cnl::make_unsigned_t<signed_multiprecision<9 >>>::value,
        "cnl::make_unsigned<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!cnl::is_signed<cnl::make_unsigned_t<unsigned_multiprecision<10 >>>::value,
        "cnl::make_unsigned<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!cnl::is_signed<cnl::make_unsigned_t<signed_multiprecision<9 >>>::value,
        "cnl::make_unsigned<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!cnl::is_signed<cnl::make_unsigned_t<unsigned_multiprecision<10 >>>::value,
        "cnl::make_unsigned<{un}signed_multiprecision>::make_unsigned test failed");

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
                    decltype(cnl::to_rep<fixed_point<unsigned_multiprecision<12>>>{}(
                            std::declval<unsigned_multiprecision<12>>()))>::value,
            "cnl::_impl::depth<fixed_point<boost::multiprecision>>");
}

namespace test_impl_to_rep {
    static_assert(
            assert_same<
                    unsigned_multiprecision<987654321>&&,
                    cnl::_impl::to_rep_t<fixed_point<unsigned_multiprecision<987654321>>>>::value,
            "cnl::_impl::depth<fixed_point<boost::multiprecision>>");
}

namespace test_depth {
    static_assert(
            cnl::_impl::depth<fixed_point<unsigned_multiprecision<987654321>>>::value==1,
            "cnl::_impl::depth<fixed_point<boost::multiprecision>>");
}

////////////////////////////////////////////////////////////////////////////////
// cnl::multiprecision arithmetic

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
    using int64 = cnl::unsigned_multiprecision<64>;

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
// cnl::fixed_point<cnl::unsigned_multiprecision<>> arithmetic

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

TEST(fixed_point_multiprecision, shift_left_constant)
{
    auto lhs = cnl::unsigned_multiprecision<15>{1536};
    auto rhs = cnl::constant<3>{};
    auto actual = lhs << rhs;
    auto expected = cnl::unsigned_multiprecision<15>{1536 << 3};
    ASSERT_EQ(expected, actual);
}

TEST(fixed_point_multiprecision, shift_right_constant)
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

TEST(fixed_point_multiprecision, unsigned_multiprecision_used_digits)
{
    using q4_20 = fixed_point<unsigned_multiprecision<24>, -20>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(6, cnl::used_digits(a));
}

TEST(fixed_point_multiprecision, signed_multiprecision_used_digits)
{
    using q4_20 = fixed_point<signed_multiprecision<24>, -20>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(6, cnl::used_digits(a));
}

////////////////////////////////////////////////////////////////////////////////
// cnl::leading_bits<boost::multiprecision::number<>>

TEST(fixed_point_multiprecision, unsigned_multiprecision_leading_bits)
{
    using q4_20 = fixed_point<unsigned_multiprecision<24>, -20>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(18, leading_bits(a));
}

TEST(fixed_point_multiprecision, signed_multiprecision_leading_bits)
{
    using q4_20 = fixed_point<signed_multiprecision<24>, -20>;
    q4_20 a = 3.051757812500000e-05;
    ASSERT_EQ(18, leading_bits(a));
}

TEST(fixed_point_multiprecision, sqrt)
{
    typedef cnl::fixed_point<boost::multiprecision::int128_t, 0> length_t;
    length_t x = length_t{ 25 };
    auto y = cnl::sqrt(x);
    ASSERT_EQ(y, 5);
}

#endif  // CNL_BOOST_ENABLED
