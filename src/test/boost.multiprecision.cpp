
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(SG14_BOOST_ENABLED)

#include <cnl/auxiliary/boost.multiprecision.h>

#include <cnl/fixed_point>

#include <gtest/gtest.h>

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
// boost::throw_exception

#if defined(BOOST_NO_EXCEPTIONS)
namespace boost {
    void throw_exception(std::exception const &) {
        std::terminate();
    }
}
#endif

#endif  // SG14_BOOST_ENABLED
