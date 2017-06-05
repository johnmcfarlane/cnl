
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(SG14_BOOST_ENABLED)

#include <sg14/auxiliary/multiprecision.h>

#include <sg14/fixed_point>

#include <gtest/gtest.h>

using sg14::fixed_point;
using sg14::multiprecision;
using sg14::numeric_traits;
using sg14::_impl::set_digits_t;
using sg14::signed_multiprecision;
using sg14::unsigned_multiprecision;

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

// test sg14::numeric_traits<{un}signed_multiprecision>::make_signed
static_assert(std::numeric_limits<numeric_traits<signed_multiprecision<9>>::make_signed>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_signed test failed");
static_assert(std::numeric_limits<numeric_traits<unsigned_multiprecision<10 >>::make_signed>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_signed test failed");
static_assert(std::numeric_limits<numeric_traits<signed_multiprecision<9 >>::make_signed>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_signed test failed");
static_assert(std::numeric_limits<numeric_traits<unsigned_multiprecision<10 >>::make_signed>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_signed test failed");

// test sg14::numeric_traits<{un}signed_multiprecision>::make_unsigned
static_assert(!std::numeric_limits<numeric_traits<signed_multiprecision<9 >>::make_unsigned>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!std::numeric_limits<numeric_traits<unsigned_multiprecision<10 >>::make_unsigned>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!std::numeric_limits<numeric_traits<signed_multiprecision<9 >>::make_unsigned>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_unsigned test failed");
static_assert(!std::numeric_limits<numeric_traits<unsigned_multiprecision<10 >>::make_unsigned>::is_signed,
        "sg14::numeric_traits<{un}signed_multiprecision>::make_unsigned test failed");

// test sg14::width<{un}signed_multiprecision>
static_assert(numeric_traits<signed_multiprecision<11>>::digits>=11, "numeric_traits<signed_multiprecision>::digits test failed");
static_assert(numeric_traits<unsigned_multiprecision<12>>::digits>=12, "numeric_traits<signed_multiprecision>::digits test failed");

// test sg14::set_digits_t<{un}signed_multiprecision>
static_assert(numeric_traits<set_digits_t<signed_multiprecision<13>, 57>>::digits>=57, "set_digits_t<signed_multiprecision> test failed");
static_assert(numeric_traits<set_digits_t<unsigned_multiprecision<14>, 3>>::digits>=3, "set_digits_t<signed_multiprecision> test failed");

static_assert(numeric_traits<set_digits_t<signed_multiprecision<120>, 3>>::digits<=numeric_traits<signed_multiprecision<120>>::digits, "set_digits_t<signed_multiprecision> test failed");
static_assert(numeric_traits<unsigned_multiprecision<128>>::digits>=numeric_traits<set_digits_t<unsigned_multiprecision<128>, 16>>::digits, "set_digits_t<signed_multiprecision> test failed");

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
