
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of fixed_point with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#include <gtest/gtest.h>

#include "common_defs.h"

#include "fractional_ctor.h"

////////////////////////////////////////////////////////////////////////////////
// copy assignment

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_fixed_point)
{
    auto lhs = fixed_point<int32, -16>(0);
    lhs = static_cast<fixed_point<int32, -16>>(123.456);
    auto expected = fixed_point<int32, -16>(123.456);
    ASSERT_EQ(expected, lhs);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_floating_point)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = 234.567;
    ASSERT_EQ(lhs, 234.56698608398438);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_integer)
{
    auto lhs = fixed_point<int32, -16>(0);
    lhs = static_cast<fixed_point<int32, -16>>(543);
    ASSERT_EQ(lhs, 543);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_fractional)
{
    auto lhs = cnl::fixed_point<test_int, -15>{0};
    lhs = cnl::make_fractional(int16{1}, int16{3});
    ASSERT_EQ(lhs, 0.33331298828125);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_alternative_specialization)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = fixed_point<uint8>(87.65);
    ASSERT_EQ(static_cast<fixed_point<uint8>>(lhs), fixed_point<uint8>(87.65));
}

////////////////////////////////////////////////////////////////////////////////
// compound assignment

namespace test_compound_assignment {
    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_f_i) {
        auto lhs = fixed_point<uint32, -16>{7};
        auto rhs = uint32{12};
        lhs += rhs;
        ASSERT_EQ(lhs, 19U);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_i_f) {
        auto lhs = int32{7};
        auto rhs = fixed_point<int32, -16>{12};
        lhs += rhs;
        ASSERT_EQ(lhs, 19);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_f_f) {
        auto rhs = fixed_point<int32, -16>{12.25};
        auto lhs = fixed_point<int64, -20>{18726.125L};
        lhs += rhs;
        ASSERT_EQ(lhs, 18738.375);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_f_i) {
        auto lhs = fixed_point<int32, -16>{7};
        auto rhs = int32{12};
        lhs -= rhs;
        ASSERT_EQ(lhs, -5);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_i_f) {
        auto lhs = 18726.125L;
        auto rhs = fixed_point<uint8, -4>{2.5};
        lhs -= rhs;
        ASSERT_EQ(lhs, 18723.625L);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_f_f) {
        auto lhs = fixed_point<int64, -6>{18726.125L};
        auto rhs = fixed_point<uint8, -4>{2.5};
        lhs -= rhs;
        ASSERT_EQ(lhs, 18723.625L);
    }
}

////////////////////////////////////////////////////////////////////////////////
// compound assignment

TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), from_compound_assignment)
{
    auto x = fixed_point<int16, -8>(22.75);
    ASSERT_EQ(x += 12.5, 35.25L);
    ASSERT_EQ(x -= 35.5, -.25);
    ASSERT_EQ(x /= 4, -.0625);
    ASSERT_EQ(x -= -10, 9.9375f);
    ASSERT_EQ(x *= -3, -29.8125);
    ASSERT_EQ(x -= 0.1875, -30);
}

////////////////////////////////////////////////////////////////////////////////
// increment/decrement operators

TEST(TOKENPASTE2(TEST_LABEL, pre), increment)
{
    auto b = fixed_point<int16, -8>(22.75);
    auto &a = ++b;
    static_assert(std::is_same<fixed_point<int16, -8>&, decltype(a)>::value, "");
    ASSERT_EQ(&b, &a);
    ASSERT_EQ(23.75, a);
    ASSERT_EQ(23.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, pre), decrement)
{
    auto b = fixed_point<int16, -8>(22.75);
    auto &a = --b;
    static_assert(std::is_same<fixed_point<int16, -8>&, decltype(a)>::value, "");
    ASSERT_EQ(&b, &a);
    ASSERT_EQ(21.75, a);
    ASSERT_EQ(21.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, post), increment)
{
    auto b = fixed_point<int16, -8>(22.75);
    auto a = b++;
    static_assert(std::is_same<fixed_point<int16, -8>, decltype(a)>::value, "");
    ASSERT_NE(&b, &a);
    ASSERT_EQ(22.75, a);
    ASSERT_EQ(23.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, post), decrement)
{
    auto b = fixed_point<int16, -8>(22.75);
    auto a = b--;
    static_assert(std::is_same<fixed_point<int16, -8>, decltype(a)>::value, "");
    ASSERT_NE(&b, &a);
    ASSERT_EQ(22.75, a);
    ASSERT_EQ(21.75, b);
}

////////////////////////////////////////////////////////////////////////////////
// sqrt exception throwing

#if defined(CNL_EXCEPTIONS_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, sqrt_exception), from_alternative_specialization)
{
    ASSERT_DEATH(sqrt(fixed_point<>(-1)), "negative value passed to cnl::sqrt\n");
}

#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// language assumptions

// default rounding style is truncation
static_assert(static_cast<int>(3.9)==3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(3.0)==3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(2.9)==2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(2.0)==2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(1.9)==1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(1.0)==1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(0.9)==0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(0.0)==0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-0.0)==0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-0.9)==0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-1.0)==-1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-1.9)==-1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-2.0)==-2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-2.9)==-2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-3.0)==-3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-3.9)==-3, "incorrect assumption about default rounding");

// promotion doesn't always tend towards int
static_assert(identical(int8(0)+int8(0), test_int{0}), "incorrect assumption about promotion");
static_assert(identical(int8(0)+int8(0), test_int{0}), "incorrect assumption about promotion");
static_assert(identical(uint8(0)+int8(0), test_int{0}), "incorrect assumption about promotion");
static_assert(identical(uint8(0)+uint8(0), test_int{0}), "incorrect assumption about promotion");

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::_impl

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::shift positive RHS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4310)
#endif

static_assert(cnl::_impl::shift<1>(int8(0))==0, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<1, 10>(int8(0))==0, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<1, 10>(int8(1))==10, "cnl::_impl::shift test failed");

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(cnl::_impl::shift<8>(uint16{0x1234})==0x123400, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<8>(uint8{0x12})==0x1200, "cnl::_impl::shift test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(identical(cnl::_impl::shift<8, 2, uint16>((uint16)0x1234), uint16{0x1234}<<8), "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<8, 2, uint16>((uint8)0x1234) == 0xff00, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<8, 2, uint8>(0x34) == test_int{0x3400}, "cnl::_impl::shift test failed");
#endif

static_assert(cnl::_impl::shift<8, 2, int16>(-123)==-31488, "cnl::_impl::shift test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::fp::type::cnl::_impl::shift negative RHS

static_assert(cnl::_impl::shift<-8, 2, uint16>((uint16) 0x1234)==0x12, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<-8, 2, int16>(-31488)==-123, "cnl::_impl::shift test failed");

static_assert(identical(test_int{0x123400}, cnl::_impl::shift<8, 2>(uint16{0x1234})), "cnl::_impl::shift test failed");

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
static_assert(cnl::_impl::shift<-8, 2, uint16>((uint16) 0x1234)==0x12, "cnl::_impl::shift test failed");
static_assert(cnl::_impl::shift<-8, 2, int16>(-31488)==-123, "cnl::_impl::shift test failed");

#if !defined(TEST_THROWING_OVERFLOW) && !defined(TEST_TRAPPING_OVERFLOW)
static_assert(cnl::_impl::shift<-8, 2, uint16>((uint8) 0x34)==0x0, "cnl::_impl::shift test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::power

namespace test_power {
    static_assert(identical(cnl::_impl::power<float, 0, 2>(), 1.f), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, -1, 2>(), .5), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 1, 2>(), 2.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, -3, 2>(), .125f), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, 7, 2>(), 128.), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 10, 2>(), 1024.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, 20, 2>(), 1048576.f), "cnl::_impl::power test failed");

    static_assert(identical(cnl::_impl::power<float, 0, 3>(), 1.f), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, -1, 3>(), 1./3), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 1, 3>(), 3.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, -3, 3>(), 1.f/27), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, 7, 3>(), 2187.), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 10, 3>(), 59049.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, 20, 3>(), 3486784401.f), "cnl::_impl::power test failed");

    static_assert(identical(cnl::_impl::power<float, 0, 4>(), 1.f), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, -1, 4>(), .25), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 1, 4>(), 4.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, -3, 4>(), .015625f), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<double, 7, 4>(), 16384.), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<long double, 10, 4>(), 1048576.L), "cnl::_impl::power test failed");
    static_assert(identical(cnl::_impl::power<float, 20, 4>(), 1099511627776.f), "cnl::_impl::power test failed");
}

////////////////////////////////////////////////////////////////////////////////
// cnl::fixed_point<>::fixed_point

namespace ctor {
    static_assert(identical(fixed_point<uint64>{123}, fixed_point<uint64>(123)), "fixed_point<>::fixed_point");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(fixed_point<uint128, -16>(fixed_point<uint64>{123}), fixed_point<uint128, -16>(123)), "fixed_point<>::fixed_point");
#endif

#if !defined(TEST_THROWING_OVERFLOW) && !defined(TEST_TRAPPING_OVERFLOW) && !defined(TEST_SATURATED_OVERFLOW)
    // the equivalent test in elastic_number.cpp does not lose information
    static_assert(identical(uint32{0x00003210U}, uint32(fixed_point<uint64, -16>{0x76543210U})), "fixed_point<>::fixed_point");
#endif

    static_assert(identical(to_rep(fixed_point<int, 2>{4}), 1), "to_rep<fixed_point<int, 2>>()");
}

////////////////////////////////////////////////////////////////////////////////
// numeric traits

namespace test_is_composite {
    using cnl::is_composite;

    static_assert(is_composite<fixed_point<test_int>>::value, "cnl::is_composite<fixed_point<>> test failed");
    static_assert(cnl::_impl::are_composite<fixed_point<test_int>>::value, "cnl::is_composite<fixed_point<>> test failed");
}

namespace test_to_rep {
    static_assert(identical(to_rep(fixed_point<test_int>{97531}), test_int{97531}), "to_rep");
    static_assert(identical(to_rep(fixed_point<uint8>{199}), uint8{199}), "to_rep");
}

namespace test_from_rep {
    static_assert(! cnl::from_rep<cnl::fixed_point<unsigned int, -33>>{}(0), "from_rep");
    static_assert(identical(cnl::from_rep<fixed_point<>>{}(test_int{0}), fixed_point<>{0}), "from_rep");
    static_assert(identical(cnl::from_rep<fixed_point<int16, -10>>{}(int16{3072}), fixed_point<int16, -10>{test_int{3}}), "from_rep");
    static_assert(!cnl::from_rep<fixed_point<test_int, -100>>{}(test_int{0}), "from_rep");
    static_assert(cnl::from_rep<fixed_point<test_int, -100>>{}(test_int{1}), "from_rep");
    static_assert(!cnl::from_rep<fixed_point<test_int, 1000>>{}(test_int{0}), "from_rep");
    static_assert(cnl::from_rep<fixed_point<test_int, 1000>>{}(test_int{1}), "from_rep");
}

namespace test_impl_make_number {
    static_assert(identical(fixed_point<short>{123}, cnl::_impl::make_number<fixed_point<long long>>(short{123})),
            "cnl::_impl::make_number<fixed_point<>>");
    static_assert(identical(fixed_point<std::uint64_t>{404}, cnl::_impl::make_number<fixed_point<>>(UINT64_C(404))),
            "cnl::_impl::make_number<fixed_point<>, cnl::constant<4>>()");

    static_assert(identical(cnl::_impl::make_number<fixed_point<int32>>(cnl::constant<369>{}), fixed_point<int>{369}),
            "cnl::_impl::make_number<fixed_point<>>");
    static_assert(identical(fixed_point<int, 2>{4}, cnl::_impl::make_number<fixed_point<>>(cnl::constant<4>{})),
            "cnl::_impl::make_number<fixed_point<>, cnl::constant<4>>()");
}

#if defined(__cpp_deduction_guides)
namespace test_deduction_guides {
    static_assert(identical(cnl::make_fixed_point(short{123}), cnl::fixed_point(short{123})),
            "cnl::fixed_point class template deduction");
    static_assert(identical(cnl::make_fixed_point(UINT64_C(404)), cnl::fixed_point(UINT64_C(404))),
            "cnl::fixed_point class template deduction");

    static_assert(identical(cnl::make_fixed_point(cnl::constant<369>{}), cnl::fixed_point(cnl::constant<369>{})),
            "cnl::fixed_point class template deduction");
    static_assert(identical(cnl::make_fixed_point(cnl::constant<4>{}), cnl::fixed_point(cnl::constant<4>{})),
            "cnl::fixed_point class template deduction");
}
#else
namespace test_make_fixed_point {
    static_assert(identical(fixed_point<short>{123}, cnl::make_fixed_point(short{123})),
            "cnl::make_fixed_point");
    static_assert(identical(fixed_point<std::uint64_t>{404}, cnl::make_fixed_point(UINT64_C(404))),
            "cnl::make_fixed_point");

    static_assert(identical(fixed_point<int>{369}, cnl::make_fixed_point(cnl::constant<369>{})),
            "cnl::make_fixed_point");
    static_assert(identical(fixed_point<int, 2>{4}, cnl::make_fixed_point(cnl::constant<4>{})),
            "cnl::make_fixed_point");
}
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::set_digits_t<fixed_point<>, >

namespace test_set_digits_t {
    template<class Type, cnl::_digits_type MinNumDigits>
    using set_digits_t = cnl::set_digits_t<Type, MinNumDigits>;

    static_assert(identical(set_digits_t<fixed_point<uint8, -8>, 8>{0}, fixed_point < uint8, -8 > {0}),
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int8, 8>, 15>, fixed_point<int16, 8>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint16, -16>, 24>, fixed_point<uint32, -16>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int16, 16>, 31>, fixed_point<int32, 16>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint32, -45>, 40>, fixed_point<uint64, -45>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int32, -8>, 47>, fixed_point<int64, -8>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint64, 8>, 56>, fixed_point<uint64, 8>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int64, -16>, 63>, fixed_point<int64, -16>>::value,
                  "cnl::set_digits_t test failed");
#if defined(CNL_INT128_ENABLED)
    static_assert(is_same<set_digits_t<fixed_point<uint8, 16>, 72>, fixed_point<uint128, 16>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int8, -45>, 79>, fixed_point<int128, -45>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint16, -8>, 88>, fixed_point<uint128, -8>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int16, 8>, 95>, fixed_point<int128, 8>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint32, -16>, 104>, fixed_point<uint128, -16>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int32, 16>, 111>, fixed_point<int128, 16>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<uint64, -45>, 120>, fixed_point<uint128, -45>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<fixed_point<int64, -8>, 127>, fixed_point<int128, -8>>::value,
                  "cnl::set_digits_t test failed");
#if defined(__GNUC__)
// GCC complains about __int128 with -pedantic or -pedantic-errors
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
    static_assert(
            is_same<cnl::set_digits_t<cnl::fixed_point<long unsigned int, -96>, 96u>, cnl::fixed_point<cnl::uint128, -96>>::value,
            "");
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::fixed_point

////////////////////////////////////////////////////////////////////////////////
// default second template parameter

static_assert(is_same<fixed_point<int8>, fixed_point<int8, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<uint8>, fixed_point<uint8, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<int16>, fixed_point<int16, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<uint16>, fixed_point<uint16, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<int32>, fixed_point<int32, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<uint32>, fixed_point<uint32, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<int64>, fixed_point<int64, 0>>::value, "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<uint64>, fixed_point<uint64, 0>>::value, "cnl::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// default first template parameter

static_assert(identical(fixed_point<test_int, 0>{test_int{ 0 }}, fixed_point<>{test_int{ 0 }}), "cnl::fixed_point test failed");
static_assert(is_same<fixed_point<test_int, 0>, fixed_point<>>::value, "cnl::fixed_point test failed");

namespace test_glvalue_error {
#if ! defined(__GNUG__) // Bug 71504?
    constexpr auto q = fixed_point<test_int, 0>{1}/1;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// conversion

// exponent == 0
static_assert(fixed_point<uint8>(12.34f)==12.f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint16>(12.34f)==12.f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint32>(12.34f)==12.L, "cnl::fixed_point test failed");
static_assert(fixed_point<uint64>(12.34f)==12.f, "cnl::fixed_point test failed");

static_assert((fixed_point<int8>(-12.34f))==-12.f, "cnl::fixed_point test failed");
static_assert(fixed_point<int16>(-12.34f)==-12.L, "cnl::fixed_point test failed");
static_assert(fixed_point<int32>(-12.34f)==-12.f, "cnl::fixed_point test failed");
static_assert((fixed_point<int64>(-12.34f))==-12.f, "cnl::fixed_point test failed");

// exponent = -1
static_assert(fixed_point<uint8, -1>(127.5)==127.5, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, -1>(63.5)==63.5, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, -1>(-63.5)==-63.5, "cnl::fixed_point test failed");

// exponent == -7
static_assert(fixed_point<uint8, -7>(.125f)==.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f)==232.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint32, -7>(232.125f)==232.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f)==232.125f, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, -7>(.125f)==.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<int16, -7>(123.125f)==123.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<int32, -7>(123.125f)==123.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125f)==123.125f, "cnl::fixed_point test failed");

static_assert((fixed_point<uint8, -7>(.125f))==.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f)==232.125L, "cnl::fixed_point test failed");
static_assert((fixed_point<uint32, -7>(232.125f))==232.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f)==232.125L, "cnl::fixed_point test failed");

#if !defined(TEST_THROWING_OVERFLOW) && !defined(TEST_TRAPPING_OVERFLOW)
static_assert(fixed_point<int8, -7>(1)!=1.L, "cnl::fixed_point test failed");
#endif

static_assert(fixed_point<int8, -7>(.5)==.5f, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, -7>(.125f)==.125L, "cnl::fixed_point test failed");
static_assert(fixed_point<int16, -7>(123.125f)==123.125f, "cnl::fixed_point test failed");
static_assert(fixed_point<int32, -7>(123.125f)==123.125, "cnl::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125l)==123.125f, "cnl::fixed_point test failed");

// exponent == 16
static_assert(fixed_point<uint8, 16>(test_int{ 65536 }) == 65536.f, "cnl::fixed_point test failed");
static_assert(fixed_point<uint16, 16>(6553.)==0, "cnl::fixed_point test failed");
static_assert((fixed_point<uint32, 16>(4294967296l))==4294967296.f, "cnl::fixed_point test failed");
static_assert((fixed_point<uint64, 16>(1125895611875328l))==1125895611875328ul, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, 16>(-65536)==-65536.f, "cnl::fixed_point test failed");
static_assert(fixed_point<int16, 16>(-6553.)==0, "cnl::fixed_point test failed");
static_assert((fixed_point<int32, 16>(-4294967296l))==-4294967296.f, "cnl::fixed_point test failed");
static_assert((fixed_point<int64, 16>(-0x800000000000LL))==-0x800000000000LL, "cnl::fixed_point test failed");

// exponent = 1
static_assert(fixed_point<uint8, 1>(10)==10, "cnl::fixed_point test failed");
static_assert(fixed_point<uint8, 1>(11)==10, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(123.5)==122, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, 1>(127)==126, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(126)==126, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(-5)==-4, "cnl::fixed_point test failed");

// conversion between fixed_point specializations
static_assert(fixed_point<uint8, -4>(fixed_point<int16, -8>(1.5))==1.5, "cnl::fixed_point test failed");
#if !defined(TEST_THROWING_OVERFLOW) && !defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_TRAPPING_OVERFLOW)
static_assert(fixed_point<uint16, -8>(fixed_point<int8, -4>(3.25))==3.25, "cnl::fixed_point test failed");
static_assert(fixed_point<uint8, 4>(fixed_point<int16, -4>(768))==768, "cnl::fixed_point test failed");
static_assert(fixed_point<uint32, -24>(fixed_point<uint64, -48>(3.141592654))>3.1415923f,
        "cnl::fixed_point test failed");
static_assert(fixed_point<uint32, -24>(fixed_point<uint64, -48>(3.141592654))<3.1415927f,
        "cnl::fixed_point test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(fixed_point<>(-1), "cnl::fixed_point test failed");
static_assert(fixed_point<>(1024), "cnl::fixed_point test failed");
static_assert(!fixed_point<>(0), "cnl::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// fixed-point arithmetic support types

namespace test_arithmetic {
    using cnl::_impl::add_op;
    using cnl::_impl::multiply_op;
    using cnl::_impl::divide_op;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_fixed_point_impl::rep_op_exponent

    static_assert(identical(
            cnl::_impl::binary_operator<cnl::_impl::subtract_op, fixed_point<int32>, cnl::constant<369>>()(
                    fixed_point<int32>{999}, cnl::constant<369>{}), fixed_point<test_int, 0>{630}),
            "cnl::_fixed_point_impl::rep_op_exponent test failed");
}

////////////////////////////////////////////////////////////////////////////////
// multiplication

static_assert(cnl::numeric_limits<uint8>::max()/5==51, "");
static_assert(cnl::numeric_limits<uint8>::max()/3==85, "");

namespace test_divide {
    static_assert(
            identical(cnl::quotient(fixed_point<test_int, -14>{1}, int16{127}), fixed_point<int64, -29>{1./127}),
            "cnl::quotient test failed");
    static_assert(identical(cnl::quotient(int32(-999), int32(3)), fixed_point<int64, -31>{-333LL}),
            "cnl::fixed_point test failed");
    static_assert(identical(cnl::quotient(fixed_point<uint32, 10>{10240}, uint32{3u}), fixed_point<uint64,
            -22>{3413.3333333}), "cnl::fixed_point division test failed");
    static_assert(identical(cnl::quotient(test_int{10}, fixed_point<uint8, -2>{0.25}), fixed_point<int64, -6>{40}),
            "cnl::fixed_point division test failed");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(
            cnl::quotient(fixed_point<uint64, 0>{0xFFFFFFFE00000001LL}, fixed_point<uint64, -32>{0xffffffffULL}),
            fixed_point<uint128, -32>{0xffffffffULL}), "cnl::fixed_point test failed");
#endif
    static_assert(identical(cnl::quotient(fixed_point<uint32, 0>{0xFFFE0001LL}, fixed_point<uint32, 0>{0xffff}),
            fixed_point<uint64, -32>{0xffffLL}), "cnl::fixed_point test failed");
    static_assert(identical(
            cnl::quotient(cnl::fixed_point<int32, -16>{963}, cnl::constant<3>{}),
            fixed_point<int64, -18>{321LL}), "cnl::fixed_point test failed");
}

////////////////////////////////////////////////////////////////////////////////
// comparison

// heterogeneous fixed-point to fixed-point comparison
#if !defined(TEST_THROWING_OVERFLOW) && !defined(TEST_SATURATED_OVERFLOW)
static_assert(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(-4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<fixed_point<int16, -7>(5.6), "cnl::fixed_point test failed");
#endif
#if !defined(TEST_THROWING_OVERFLOW)
static_assert(!(fixed_point<int8, -3>(-4.5)<fixed_point<int16, -7>(-5.6)), "cnl::fixed_point test failed");
#endif

#if !defined(TEST_THROWING_OVERFLOW)
static_assert(fixed_point<uint8, -4>(4.6)>fixed_point<int16, -8>(.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.6)<fixed_point<int16, -8>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(.5), "cnl::fixed_point test failed");
static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(-4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(4.6)), "cnl::fixed_point test failed");
#endif

// heterogeneous fixed-point to arithmetic comparison
static_assert(fixed_point<uint8, -4>(4.5)==4.5, "cnl::fixed_point test failed");
static_assert(!(4==fixed_point<int16, -7>(-4.5)), "cnl::fixed_point test failed");

static_assert(4.5f!=fixed_point<int16, -7>(-4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint64, -4>(4.5)!=4.5L), "cnl::fixed_point test failed");

static_assert(4.5<fixed_point<int16, -7>(5.6), "cnl::fixed_point test failed");
static_assert(!(fixed_point<int32, -3>(-4.5)<-5.6), "cnl::fixed_point test failed");

static_assert(fixed_point<int16, -13>(3.141) > 3, "cnl::fixed_point test failed");
static_assert(fixed_point<int32, -15>{-0.2} < 0, "cnl::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert((fixed_point<int32>(123)+fixed_point<int32>(123))==246, "cnl::fixed_point addition operator test failed");
static_assert((fixed_point<int32, -16>(123.125)+fixed_point<int32, -16>(123.75))==246.875, "cnl::fixed_point addition operator test failed");
static_assert((fixed_point<int32, -16>(123.125)+fixed_point<int32, -16>(123.75))==246.875, "cnl::fixed_point addition operator test failed");

static_assert(identical(fixed_point<int8, -5>{2.125}+fixed_point<int8, -5>{-3.25}, fixed_point<test_int, -5>{-1.125f}),
        "cnl::fixed_point addition operator test failed");

static_assert(identical(fixed_point<uint8, -4>{1.5}+2048, fixed_point<test_int, -4>{2049.5}), "test failed");
static_assert(identical(2048+fixed_point<uint8, -4>{2.25}, fixed_point<test_int, -4>{2050.25}),
        "cnl::fixed_point addition operator test failed");
static_assert(is_same<decltype(2048+fixed_point<uint8, 10>(10240)), fixed_point<test_signed, 0>>::value,
        "cnl::fixed_point addition operator test failed");

static_assert(identical(16777981.428100586f, 765.432f+fixed_point<int64, -32>(16777215.996093750)),
        "cnl::fixed_point addition operator test failed");
static_assert(identical(16777981.42809375, 765.432+fixed_point<int64, -32>(16777215.996093750)),
        "cnl::fixed_point addition operator test failed");
static_assert(identical(16777981.428100586f, fixed_point<int64, -32>(16777215.996093750)+765.432f),
        "cnl::fixed_point addition operator test failed");
static_assert(identical(16777981.42809375, fixed_point<int64, -32>(16777215.996093750)+765.432),
        "cnl::fixed_point addition operator test failed");

static_assert(identical(fixed_point<int32, -16>{.5} + 2, fixed_point<test_int, -16>{2.5}),
        "cnl::fixed_point addition operator test failed");

// subtraction
static_assert((fixed_point<int32>(999)-fixed_point<int32>(369))==630, "cnl::fixed_point subtraction test failed");
static_assert((fixed_point<int32, -16>(246.875)-fixed_point<int32, -16>(123.75))==123.125, "cnl::fixed_point test failed");
static_assert((fixed_point<int16, -4>(123.125)-fixed_point<int16, -4>(246.875))==-123.75, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, -5>(2.125)-fixed_point<int8, -5>(3.25)==-1.125f, "cnl::fixed_point subtraction test failed");
static_assert(is_same<decltype(fixed_point<int8, -5>(2.125)-fixed_point<int8, -5>(-3.25)), fixed_point<test_int, -5>>::value,
        "cnl::fixed_point subtraction test failed");
static_assert(fixed_point<uint8, -3>(0.875)-2048==-2047.125, "cnl::fixed_point subtraction test failed");
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)-2048), fixed_point<test_signed, 0>>::value,
        "cnl::fixed_point subtraction test failed");
static_assert(identical(-16776450.564086914f, 765.432f-fixed_point<int64, -32>(16777215.996093750)),
        "cnl::fixed_point subtraction test failed");

// multiplication
static_assert(identical(fixed_point<uint8>{0x55} * fixed_point<uint8>{2}, fixed_point<test_int, 0>{0xaa}),
        "cnl::fixed_point test failed");

static_assert(identical(fixed_point<int64, -16>{123.75} * fixed_point<int32, -16>(44.5), fixed_point<int64, -32>{5506.875}),
        "cnl::fixed_point test failed");

static_assert(
        identical(fixed_point<int8, -5>{2.125} * fixed_point<int8, -5>{-3.25}, fixed_point<test_int, -10>{-6.90625}),
        "cnl::fixed_point multiplication test failed");

#if defined(__clang__)
static_assert(identical(fixed_point<uint8, 10>{10240} * 3u, fixed_point<test_unsigned, 10>{30720}),
        "cnl::fixed_point multiplication test failed");

static_assert(identical(3u * uint8{4}, test_unsigned{12}), "cnl::fixed_point multiplication test failed");
static_assert(identical(3u * fixed_point<uint8, 10>{10240}, fixed_point<test_unsigned, 10>{30720}),
        "cnl::fixed_point multiplication test failed");
#endif

static_assert(identical(-2074569855.5169766f, -123.654f*fixed_point<int64, -32>(16777215.996093750)),
        "cnl::fixed_point multiplication test failed");
static_assert(identical(12841817994.32203125f, 765.432f*fixed_point<int64, -32>(16777215.996093750)),
        "cnl::fixed_point multiplication test failed");
static_assert(identical(-2074569855.5169766f, fixed_point<int64, -32>(16777215.996093750)*-123.654f),
        "cnl::fixed_point multiplication test failed");
static_assert(identical(-2074569866.7809765625, fixed_point<int64, -32>(16777215.996093750)*-123.654),
        "cnl::fixed_point multiplication test failed");

// division
static_assert(identical(fixed_point<int8, -1>{63}/fixed_point<int8, -1>{-4}, fixed_point<test_int, 0>{-15.75}),
        "cnl::fixed_point test failed");
static_assert(identical(fixed_point<test_int, -1>{63}/fixed_point<int8, -1>{-4}, fixed_point<test_int, 0>{-15.75}),
        "cnl::fixed_point test failed");
static_assert(identical(fixed_point<int8, 1>{-255}/fixed_point<int8, 1>{-8}, fixed_point<test_int, 0>{31.75}),
        "cnl::fixed_point test failed");
static_assert((fixed_point<int8, 1>(-255)/fixed_point<int8, 1>(-8))==31, "cnl::fixed_point test failed");

static_assert(
        identical(fixed_point<int8, -5>{2.5}/fixed_point<int8, -5>{-4.f}, fixed_point<test_int, 0>{0}),
        "cnl::fixed_point division test failed");

static_assert(identical(test_int{10}/fixed_point<uint8, -6>(0.25), fixed_point<int32, 6>{40}),
        "cnl::fixed_point division test failed");
static_assert(
        identical(fixed_point<uint8, 10>{10240}/static_cast<uint16>(3), fixed_point<test_int, 10>{3413.3333333}),
        "cnl::fixed_point division test failed");

static_assert(identical(test_int{10}/fixed_point<uint8, -2>{0.25}, fixed_point<test_int, 2>{40}),
        "cnl::fixed_point division test failed");

static_assert(16777215.996093750/fixed_point<int64, -32>(-123.654f)==-135678.71712347874,
        "cnl::fixed_point division test failed");
static_assert(is_same<decltype(16777215.996093750/fixed_point<int64, -32>(-123.654f)), double>::value,
        "cnl::fixed_point division test failed");
static_assert(identical(-135678.71712347874f, fixed_point<int64, -32>(16777215.996093750)/-123.654f),
        "cnl::fixed_point division test failed");
static_assert(identical(-135678.7163868031, fixed_point<int64, -32>(16777215.996093750)/-123.654),
        "cnl::fixed_point division test failed");

static_assert(
        identical(fixed_point<uint32, 0>{0xffffffff}/fixed_point<uint32, 0>{0xffffffff}, fixed_point<uint32, 0>{1}),
        "cnl::fixed_point test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<uint128>::is_specialized, "");
static_assert(cnl::numeric_limits<uint128>::is_integer, "");
#endif

namespace test_bitshift {
    // dynamic
    static_assert(identical(fixed_point<test_int, -4>{2}, fixed_point<uint8, -4>{1} << 1), "bitshift test failed");
    static_assert(identical(fixed_point<test_int, -4>{.5}, fixed_point<uint8, -4>{1} >> 1), "bitshift test failed");
    static_assert(identical(fixed_point<test_int, -4>{2}, fixed_point<uint8, -4>{1} << fixed_point<>{1}), "bitshift test failed");

    // cnl::constant
    static_assert(identical(fixed_point<uint8, -3>{2}, fixed_point<uint8, -4>{1} << cnl::constant<1>{}), "bitshift test failed");
    static_assert(identical(fixed_point<uint8, -5>{.5}, fixed_point<uint8, -4>{1} << cnl::constant<-1>{}), "bitshift test failed");

    static_assert(identical(fixed_point<uint8, -5>{.5}, fixed_point<uint8, -4>{1} >> cnl::constant<1>{}), "bitshift test failed");
    static_assert(identical(fixed_point<uint8, -3>{2}, fixed_point<uint8, -4>{1} >> cnl::constant<-1>{}), "bitshift test failed");

    // const_integer
    using namespace cnl::literals;
    static_assert(identical(fixed_point<uint8, -3>{2}, fixed_point<uint8, -4>{1} << 1_c), "bitshift test failed");
    static_assert(identical(fixed_point<uint8, -5>{.5}, fixed_point<uint8, -4>{1} << -1_c), "bitshift test failed");

    static_assert(identical(fixed_point<uint8, -5>{.5}, fixed_point<uint8, -4>{1} >> 1_c), "bitshift test failed");
    static_assert(identical(fixed_point<uint8, -3>{2}, fixed_point<uint8, -4>{1} >> -1_c), "bitshift test failed");
}

namespace test_bitwise_or {
    static_assert(identical(
            fixed_point<unsigned, -16>{125.625},
            fixed_point<unsigned, -12>{125.125} | fixed_point<unsigned, -16>{77.625}),
                  "operator|(fixed_point<>,fixed_point<>)");
}

namespace test_bitwise_and {
    static_assert(identical(
            fixed_point<unsigned, -16>{77.125},
            fixed_point<unsigned, -12>{125.125} & fixed_point<unsigned, -16>{77.625}),
                  "operator&(fixed_point<>,fixed_point<>)");
}

namespace test_bitwise_xor {
    static_assert(identical(
            fixed_point<unsigned, -16>{48.5},
            fixed_point<unsigned, -12>{125.125} ^ fixed_point<unsigned, -16>{77.625}),
                  "operator^(fixed_point<>,fixed_point<>)");
}

////////////////////////////////////////////////////////////////////////////////
// cnl::numeric_limits<fixed_point<>>

template<class Rep, int Exponent, class Min, class Max, class Lowest>
constexpr bool test_numeric_limits(Min min, Max max, Lowest lowest)
{
    using fp = fixed_point<Rep, Exponent>;
    using nl = cnl::numeric_limits<fp>;
    using rnl = cnl::numeric_limits<Rep>;

    static_assert(cnl::numeric_limits<Rep>::is_specialized,
                  "cnl::numeric_limits<Rep>::is_specialized");
    static_assert(cnl::numeric_limits<Rep>::is_integer,
                  "cnl::numeric_limits<Rep>::is_integer");

    static_assert(cnl::numeric_limits<fp>::is_specialized,
                  "cnl::numeric_limits<fixed_point<Rep>>::is_specialized");

    static_assert(nl::is_specialized, "numeric_limits<fixed_point>::is_specialized");
    static_assert(nl::is_signed==rnl::is_signed, "numeric_limits<fixed_point>::is_signed");
    static_assert(!nl::is_integer, "numeric_limits<fixed_point>::is_integer");
    static_assert(nl::is_exact, "numeric_limits<fixed_point>::is_exact");
    static_assert(!nl::has_infinity, "numeric_limits<fixed_point>::has_infinity");
    static_assert(!nl::has_quiet_NaN, "numeric_limits<fixed_point>::has_quiet_NaN");
    static_assert(!nl::has_signaling_NaN, "numeric_limits<fixed_point>::has_signaling_NaN");
    static_assert(!nl::has_denorm, "numeric_limits<fixed_point>::has_denorm");
    static_assert(!nl::has_denorm_loss, "numeric_limits<fixed_point>::has_denorm_loss");
    static_assert(nl::round_style==std::round_toward_zero, "numeric_limits<fixed_point>::round_style");
    static_assert(!nl::is_iec559, "numeric_limits<fixed_point>::is_iec559");
    static_assert(nl::is_bounded, "numeric_limits<fixed_point>::is_bounded");
    static_assert(nl::is_modulo==rnl::is_modulo, "numeric_limits<fixed_point>::is_modulo");
    static_assert(nl::digits==rnl::digits, "numeric_limits<fixed_point>::digits");
    static_assert(nl::digits10==rnl::digits10, "numeric_limits<fixed_point>::digits10");
    static_assert(nl::max_digits10==rnl::max_digits10, "numeric_limits<fixed_point>::max_digits10");
    static_assert(nl::radix==2, "numeric_limits<fixed_point>::radix");
    static_assert(nl::min_exponent==rnl::min_exponent, "numeric_limits<fixed_point>::min_exponent");
    static_assert(nl::min_exponent10==rnl::min_exponent10, "numeric_limits<fixed_point>::min_exponent10");
    static_assert(nl::max_exponent==rnl::max_exponent, "numeric_limits<fixed_point>::max_exponent");
    static_assert(nl::max_exponent10==rnl::max_exponent10, "numeric_limits<fixed_point>::max_exponent10");
    static_assert(nl::traps==rnl::traps, "numeric_limits<fixed_point>::traps");
    static_assert(!nl::tinyness_before, "numeric_limits<fixed_point>::tinyness_before");
    static_assert(nl::round_error()==static_cast<Rep>(0), "numeric_limits<fixed_point>::round_error");
    static_assert(nl::infinity()==rnl::infinity(), "numeric_limits<fixed_point>::infinity");
    static_assert(nl::quiet_NaN()==Rep(0), "numeric_limits<fixed_point>::quiet_NaN");
    static_assert(nl::signaling_NaN()== Rep(0), "numeric_limits<fixed_point>::signaling_NaN");

    return nl::min()==min
            && nl::lowest()==lowest
            && nl::max()==max
            && nl::epsilon()==min
            && nl::denorm_min()==min;
}

static_assert(cnl::numeric_limits<fixed_point<test_int, -256>>::lowest() < -.1e-67, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, -256>>::min() > 0., "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, -256>>::min() < .1e-76, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, -256>>::max() > .1e-67, "cnl::numeric_limits<fixed_point> test failed");

static_assert(cnl::numeric_limits<fixed_point<test_unsigned, -256>>::lowest() == 0., "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, -256>>::min() > 0., "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, -256>>::min() < .1e-76, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, -256>>::max() > .1e-67, "cnl::numeric_limits<fixed_point> test failed");

static_assert(test_numeric_limits<test_signed, -16>(1/65536.,
        cnl::numeric_limits<test_signed>::max()/65536.,
        cnl::numeric_limits<test_signed>::lowest()/65536.), "");

static_assert(test_numeric_limits<test_unsigned, -16>(1/65536.,
        cnl::numeric_limits<test_unsigned>::max()/65536.,
        cnl::numeric_limits<test_unsigned>::lowest()/65536.), "");

static_assert(test_numeric_limits<test_signed, 0>(1,
        cnl::numeric_limits<test_signed>::max(),
        cnl::numeric_limits<test_signed>::lowest()), "");

static_assert(test_numeric_limits<test_unsigned, 0U>(1U,
        cnl::numeric_limits<test_unsigned>::max(),
        cnl::numeric_limits<test_unsigned>::lowest()), "");

static_assert(test_numeric_limits<test_signed, 16>(65536.,
        cnl::numeric_limits<test_signed>::max()*65536.,
        cnl::numeric_limits<test_signed>::lowest()*65536.), "");

static_assert(test_numeric_limits<test_unsigned, 16>(65536.,
        cnl::numeric_limits<test_unsigned>::max()*65536.,
        cnl::numeric_limits<test_unsigned>::lowest()*65536.), "");

static_assert(cnl::numeric_limits<fixed_point<test_int, 256>>::lowest() < -1.e86, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, 256>>::min() > 1.e77, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, 256>>::min() < 1.e78, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_int, 256>>::max() > 1.e86, "cnl::numeric_limits<fixed_point> test failed");

static_assert(cnl::numeric_limits<fixed_point<test_unsigned, 256>>::lowest() == 0., "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, 256>>::min() > 1.e77, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, 256>>::min() < 1.e78, "cnl::numeric_limits<fixed_point> test failed");
static_assert(cnl::numeric_limits<fixed_point<test_unsigned, 256>>::max() > 1.e86, "cnl::numeric_limits<fixed_point> test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::sqrt

static_assert(sqrt(fixed_point<uint8>(225))==15, "cnl::sqrt test failed");
static_assert(sqrt(fixed_point<int8>(81))==9, "cnl::sqrt test failed");

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(sqrt(fixed_point<uint8, -1>(4))==2, "cnl::sqrt test failed");
static_assert(to_rep(fixed_point<int8, -2>(9))==36, "cnl::sqrt test failed");
static_assert(sqrt(fixed_point<int, -2>(9))==3, "cnl::sqrt test failed");
static_assert(sqrt(fixed_point<int8, -2>(9))==3, "cnl::sqrt test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(sqrt(fixed_point<uint8, -4>(4))==2, "cnl::sqrt test failed");
static_assert(static_cast<float>(sqrt(fixed_point<int32, -24>(3.141592654)))>1.7724537849426, "cnl::sqrt test failed");
static_assert(static_cast<float>(sqrt(fixed_point<int32, -24>(3.141592654)))<1.7724537849427, "cnl::sqrt test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// std::leading_bits<fixed_point>

namespace test_used_digits {
    static_assert(cnl::used_digits(fixed_point<uint8, -4>{1}) == 5, "cnl::used_digits");
    static_assert(cnl::used_digits(fixed_point<uint8, -3>{2}) == 5, "cnl::used_digits");
}

namespace test_leading_bits {
    using cnl::leading_bits;

    static_assert(leading_bits(fixed_point<uint8, -4>{1}) == 3, "cnl::leading_bits");
    static_assert(leading_bits(fixed_point<uint8, -3>{2}) == 3, "cnl::leading_bits");
    static_assert(leading_bits(fixed_point<int32, 10>{4096}) == 28, "cnl::leading_bits");
}

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/cDZIcB1LNfE/heaucUIAAgAJ

static constexpr fixed_point<int32, -16> x{1.5};
static constexpr auto y = 6.5*x - 4;
static_assert(y == 5.75, "usage test failed");

////////////////////////////////////////////////////////////////////////////////
// FixedPointTester

template <class Rep, int Exponent>
struct FixedPointTesterOutsize {
    using fixed_point = ::fixed_point<Rep, Exponent>;

    // Rep
    using rep = Rep;
    static_assert(sizeof(rep) == sizeof(fixed_point), "fixed_point must be the same size as its Rep");
    static_assert(
            is_same<rep, typename fixed_point::rep>::value,
            "mismatched rep");    // possibly overly restrictive (e.g. hardware-specific specializations)

    // Exponent
    static constexpr int exponent = Exponent;
    static_assert(
            exponent == fixed_point::exponent,
            "mismatched exponent");

    // simply assignment to and from underlying representation
    using numeric_limits = cnl::numeric_limits<fixed_point>;
    static constexpr fixed_point min = cnl::from_rep<fixed_point>{}(rep(1));
    static_assert(to_rep(min) == rep(1), "all Rep types should be able to store the number 1!");

    // unary common_type_t
    static_assert(is_same<
                    cnl::_impl::common_type_t<fixed_point>,
                    ::fixed_point<
                            typename std::common_type<Rep>::type,
                            Exponent>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    static_assert(
            is_same<
                    fixed_point,
                    cnl::_impl::common_type_t<fixed_point>>::value,
            "... and that rule should be to do nothing very exciting at all");

    // binary common_type_t
    static_assert(
            is_same<
                    fixed_point,
                    cnl::_impl::common_type_t<fixed_point, fixed_point>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    // for convenience, fixed_point API assumes binary and unary homogeneous common_types are the same
    static_assert(
            is_same<
                    cnl::_impl::common_type_t<fixed_point>,
                    cnl::_impl::common_type_t<fixed_point, fixed_point>>::value,
            "bad assumption about binary cnl::_impl::common_type_t");

    // test promotion rules for arithmetic
    static_assert(
            is_same<
                    decltype(min + min),
                    ::fixed_point<decltype(declval<rep>() + declval<rep>()), exponent>>::value,
            "promotion rule for addition fixed_point<Rep> should match its Rep");
    static_assert(
            is_same<
                    decltype(min - min),
                    ::fixed_point<decltype(declval<rep>() - declval<rep>()), exponent>>::value,
            "promotion rule for subtraction fixed_point<Rep> should match its Rep");
};

// some tests cannot be performed at run-time because
// they involve bit-shifts that exceed the capacity of the underlying type
template<class Rep, int Exponent>
struct FixedPointTester : public FixedPointTesterOutsize<Rep, Exponent> {
    using super = FixedPointTesterOutsize<Rep, Exponent>;
    using fixed_point = typename super::fixed_point;
    static constexpr fixed_point min = super::min;

#if defined(__clang__) && defined(TEST_NATIVE_OVERFLOW)
    // assorted tests of +, -, * and /
    static_assert(min + min == 2 * min, "basic arithmetic isn't working");
    static_assert(84 * min / 84 == min, "basic arithmetic isn't working");
    static_assert((7 * min) - (4 * min) == 6 * min / 2, "basic arithmetic isn't working");
#endif
};

template <typename Rep>
struct FixedPointRepTester {
    FixedPointTesterOutsize<Rep, -100> _0;
    FixedPointTester<Rep, -10> _1;
    FixedPointTester<Rep, -1> _2;
    FixedPointTester<Rep, 0> _3;
    FixedPointTester<Rep, 1> _4;
    FixedPointTester<Rep, 10> _5;
    FixedPointTesterOutsize<Rep, 100> _6;

    // test deduction guides
#if defined(__cpp_deduction_guides)
    static_assert(identical(cnl::fixed_point{Rep{0}}, cnl::fixed_point<Rep, 0>{0}));
    static_assert(identical(cnl::fixed_point(Rep{0}), cnl::fixed_point<Rep, 0>(0)));

    static_assert(identical(cnl::make_fixed_point(Rep{0}), cnl::fixed_point(Rep{0})));
#endif
};

template struct FixedPointRepTester<int8>;
template struct FixedPointRepTester<uint8>;

template struct FixedPointRepTester<int16>;
template struct FixedPointRepTester<uint16>;

template struct FixedPointRepTester<int32>;
template struct FixedPointRepTester<uint32>;

template struct FixedPointRepTester<int64>;
template struct FixedPointRepTester<uint64>;
