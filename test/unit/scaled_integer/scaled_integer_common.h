
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of scaled_integer with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#if !defined(TEST_SCALED_INTEGER_SCALED_INTEGER_COMMON_H)
#define TEST_SCALED_INTEGER_SCALED_INTEGER_COMMON_H

#include <cnl/fraction.h>

#include <gtest/gtest.h>

#include "common_defs.h"

#include "fraction_ctor.h"
#include "to_chars.h"
#include "to_string.h"

////////////////////////////////////////////////////////////////////////////////
// copy assignment

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_scaled_integer)  // NOLINT
{
    auto lhs = scaled_integer<int32, cnl::power<-16>>(0);
    lhs = static_cast<scaled_integer<int32, cnl::power<-16>>>(123.456);
    auto expected = scaled_integer<int32, cnl::power<-16>>(123.456);
    ASSERT_EQ(expected, lhs);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_floating_point)  // NOLINT
{
    auto lhs = scaled_integer<uint32, cnl::power<-16>>(0);
    lhs = 234.567;
    ASSERT_EQ(lhs, 234.56698608398438);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_integer)  // NOLINT
{
    auto lhs = scaled_integer<int32, cnl::power<-16>>(0);
    lhs = static_cast<scaled_integer<int32, cnl::power<-16>>>(543);
    ASSERT_EQ(lhs, 543);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_fractional)  // NOLINT
{
    auto lhs = cnl::scaled_integer<test_int, cnl::power<-15>>{0};
    lhs = cnl::make_fraction(int16{1}, int16{3});
    ASSERT_EQ(lhs, 0.33331298828125);
}

#if !defined(TEST_WIDE_INTEGER_8)
TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_alternative_specialization)  // NOLINT
{
    auto lhs = scaled_integer<uint32, cnl::power<-16>>(0);
    lhs = scaled_integer<uint8>(87.65);
    ASSERT_EQ(static_cast<scaled_integer<uint8>>(lhs), scaled_integer<uint8>(87.65));
}
#endif

////////////////////////////////////////////////////////////////////////////////
// compound assignment

namespace test_compound_assignment {
    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_f_i) {  // NOLINT
        auto lhs = scaled_integer<uint32, cnl::power<-16>>{7};
        auto rhs = uint32{12};
        lhs += rhs;
        ASSERT_EQ(lhs, 19U);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_i_f) {  // NOLINT
        auto lhs = int32{7};
        auto rhs = scaled_integer<int32, cnl::power<-16>>{12};
        lhs += rhs;
        ASSERT_EQ(lhs, 19);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), add_f_f) {  // NOLINT
        auto rhs = scaled_integer<int32, cnl::power<-16>>{12.25};
        auto lhs = scaled_integer<int64, cnl::power<-20>>{18726.125L};
        lhs += rhs;
        ASSERT_EQ(lhs, 18738.375);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_f_i) {  // NOLINT
        auto lhs = scaled_integer<int32, cnl::power<-16>>{7};
        auto rhs = int32{12};
        lhs -= rhs;
        ASSERT_EQ(lhs, -5);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_i_f) {  // NOLINT
        auto lhs = 18726.125L;
        auto rhs = scaled_integer<uint8, cnl::power<-4>>{2.5};
        lhs -= rhs;
        ASSERT_EQ(lhs, 18723.625L);
    }

    TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), sub_f_f) {  // NOLINT
        auto lhs = scaled_integer<int64, cnl::power<-6>>{18726.125L};
        auto rhs = scaled_integer<uint8, cnl::power<-4>>{2.5};
        lhs -= rhs;
        ASSERT_EQ(lhs, 18723.625L);
    }
}

////////////////////////////////////////////////////////////////////////////////
// compound assignment

TEST(TOKENPASTE2(TEST_LABEL, compound_assignment), from_compound_assignment)  // NOLINT
{
    auto x = scaled_integer<int16, cnl::power<-8>>(22.75);
    ASSERT_EQ(x += 12.5, 35.25L);
    ASSERT_EQ(x -= 35.5, -.25);
    ASSERT_EQ(x /= 4, -.0625);
    ASSERT_EQ(x -= -10, 9.9375F);
    ASSERT_EQ(x *= -3, -29.8125);
    ASSERT_EQ(x -= 0.1875, -30);
}

////////////////////////////////////////////////////////////////////////////////
// increment/decrement operators

TEST(TOKENPASTE2(TEST_LABEL, pre), increment)  // NOLINT
{
    auto b = scaled_integer<int16, cnl::power<-8>>(22.75);
    auto &a = ++b;
    static_assert(std::is_same<scaled_integer<int16, cnl::power<-8>>&, decltype(a)>::value, "");
    ASSERT_EQ(&b, &a);
    ASSERT_EQ(23.75, a);
    ASSERT_EQ(23.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, pre), decrement)  // NOLINT
{
    auto b = scaled_integer<int16, cnl::power<-8>>(22.75);
    auto &a = --b;
    static_assert(std::is_same<scaled_integer<int16, cnl::power<-8>>&, decltype(a)>::value, "");
    ASSERT_EQ(&b, &a);
    ASSERT_EQ(21.75, a);
    ASSERT_EQ(21.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, post), increment)  // NOLINT
{
    auto b = scaled_integer<int16, cnl::power<-8>>(22.75);
    auto a = b++;
    static_assert(std::is_same<scaled_integer<int16, cnl::power<-8>>, decltype(a)>::value, "");
    ASSERT_NE(&b, &a);
    ASSERT_EQ(22.75, a);
    ASSERT_EQ(23.75, b);
}

TEST(TOKENPASTE2(TEST_LABEL, post), decrement)  // NOLINT
{
    auto b = scaled_integer<int16, cnl::power<-8>>(22.75);
    auto a = b--;
    static_assert(std::is_same<scaled_integer<int16, cnl::power<-8>>, decltype(a)>::value, "");
    ASSERT_NE(&b, &a);
    ASSERT_EQ(22.75, a);
    ASSERT_EQ(21.75, b);
}

////////////////////////////////////////////////////////////////////////////////
// sqrt exception throwing

#if defined(CNL_EXCEPTIONS_ENABLED) && !defined(CNL_UNREACHABLE_UB_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, sqrt_exception), from_alternative_specialization)  // NOLINT
{
    ASSERT_DEATH((void)sqrt(scaled_integer<>(-1)), "negative value passed to cnl::sqrt\n");
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::_impl

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::scale positive RHS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4310)
#endif

static_assert(cnl::_impl::scale<1>(int8(0))==0, "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<1, 10>(int8(0))==0, "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<1, 10>(int8(1))==10, "cnl::_impl::scale test failed");

#if defined(TEST_NATIVE_INTEGER)
static_assert(cnl::_impl::scale<8>(uint16{0x1234})==0x123400, "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<8>(uint8{0x12})==0x1200, "cnl::_impl::scale test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW_INTEGER)
static_assert(identical(cnl::_impl::scale<8, 2, uint16>(uint16{0x1234}), uint16{0x1234}<<8), "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<8, 2, uint16>(uint8{0x1234}) == 0xff00, "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<8, 2, uint8>(0x34) == test_int{0x3400}, "cnl::_impl::scale test failed");
#endif

static_assert(cnl::_impl::scale<8, 2, int16>(-123)==-31488, "cnl::_impl::scale test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::fp::type::cnl::_impl::scale negative RHS

static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                decltype(std::declval<uint16>()/(std::declval<uint16>() << 8)){0x12},
                cnl::_impl::scale<-8, 2, uint16>(0x1234)),
        "cnl::_impl::scale test failed");
static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                decltype(std::declval<uint16>()/(std::declval<uint16>() << 8)){0x12},
                cnl::_impl::scale<-8, 2, uint16>(uint16{0x1234})),
        "cnl::_impl::scale test failed");
static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                decltype(std::declval<uint16>()/(std::declval<uint16>() << 8)){0},
                cnl::_impl::scale<-8, 2, uint16>(uint8{0x34})),
        "cnl::_impl::scale test failed");
static_assert(cnl::_impl::scale<-8, 2, int16>(-31488)==-123, "cnl::_impl::scale test failed");

static_assert(
        identical(
                decltype(std::declval<uint16>()*(std::declval<uint16>() << 8)){0x123400},
                cnl::_impl::scale<8, 2>(uint16{0x1234})),
        "cnl::_impl::scale test failed");

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
static_assert(cnl::_impl::scale<-8, 2, int16>(-31488)==-123, "cnl::_impl::scale test failed");

////////////////////////////////////////////////////////////////////////////////
// power_value

namespace test_power_value {
    static_assert(identical(cnl::_impl::power_value<float, 0, 2>(), 1.F), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, -1, 2>(), .5), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 1, 2>(), 2.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, -3, 2>(), .125F), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, 7, 2>(), 128.), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 10, 2>(), 1024.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, 20, 2>(), 1048576.F), "cnl::_impl::power_value test failed");

    static_assert(identical(cnl::_impl::power_value<float, 0, 3>(), 1.F), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, -1, 3>(), 1./3), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 1, 3>(), 3.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, -3, 3>(), 1.F/27), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, 7, 3>(), 2187.), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 10, 3>(), 59049.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, 20, 3>(), 3486784401.F), "cnl::_impl::power_value test failed");

    static_assert(identical(cnl::_impl::power_value<float, 0, 4>(), 1.F), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, -1, 4>(), .25), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 1, 4>(), 4.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, -3, 4>(), .015625F), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<double, 7, 4>(), 16384.), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<long double, 10, 4>(), 1048576.L), "cnl::_impl::power_value test failed");
    static_assert(identical(cnl::_impl::power_value<float, 20, 4>(), 1099511627776.F), "cnl::_impl::power_value test failed");
}

////////////////////////////////////////////////////////////////////////////////
// cnl::scaled_integer<>::scaled_integer

namespace ctor {
    static_assert(identical(scaled_integer<uint64>{123}, scaled_integer<uint64>(123)), "scaled_integer<>::scaled_integer");
#if defined(CNL_INT128_ENABLED)
    static_assert(identical(scaled_integer<uint128, cnl::power<-16>>(scaled_integer<uint64>{123}), scaled_integer<uint128, cnl::power<-16>>(123)), "scaled_integer<>::scaled_integer");
#endif

#if defined(TEST_NATIVE_INTEGER)
    // the equivalent test in elastic_scaled_integer.cpp does not lose information
    static_assert(identical(uint32{0x00003210U}, uint32(scaled_integer<uint64, cnl::power<-16>>{0x76543210U})), "scaled_integer<>::scaled_integer");
#endif

    static_assert(identical(cnl::_impl::to_rep(scaled_integer<int, cnl::power<2>>{4}), 1), "cnl::_impl::to_rep<scaled_integer<int, cnl::power<2>>>()");
}

////////////////////////////////////////////////////////////////////////////////
// numeric traits

namespace test_is_composite {
    using cnl::is_composite;

    static_assert(is_composite<scaled_integer<test_int>>::value, "cnl::is_composite<scaled_integer<>> test failed");
    static_assert(cnl::_impl::are_composite<scaled_integer<test_int>>::value, "cnl::is_composite<scaled_integer<>> test failed");
}

namespace test_to_rep {
    static_assert(identical(cnl::_impl::to_rep(scaled_integer<test_int>{97531}), test_int{97531}), "cnl::_impl::to_rep");
    static_assert(identical(cnl::_impl::to_rep(scaled_integer<uint8>{199}), uint8{199}), "cnl::_impl::to_rep");
}

namespace test_from_rep {
    static_assert(!cnl::_impl::from_rep<cnl::scaled_integer<unsigned int, cnl::power<3>>>(0), "from_rep");
    static_assert(identical(cnl::_impl::from_rep<scaled_integer<>>(test_int{0}), scaled_integer<>{0}), "from_rep");
    static_assert(
            identical(
                    cnl::_impl::from_rep<scaled_integer<int16, cnl::power<-10>>>(int16{3072}),
                    scaled_integer<int16, cnl::power<-10>>{test_int{3}}),
            "from_rep");
    static_assert(!cnl::_impl::from_rep<scaled_integer<test_int, cnl::power<-100>>>(test_int{0}), "from_rep");
    static_assert(cnl::_impl::from_rep<scaled_integer<test_int, cnl::power<-100>>>(test_int{1}), "from_rep");
    static_assert(!cnl::_impl::from_rep<scaled_integer<test_int, cnl::power<1000>>>(test_int{0}), "from_rep");
    static_assert(cnl::_impl::from_rep<scaled_integer<test_int, cnl::power<1000>>>(test_int{1}), "from_rep");
}

namespace test_from_value {
    static_assert(
            identical(
                    scaled_integer<>{3},
                    cnl::from_value<scaled_integer<test_int, cnl::power<-1>>, test_int>{}(3)),
            "");
    static_assert(identical(scaled_integer<short>{123}, cnl::_impl::from_value<scaled_integer<long long>>(short{123})),
            "cnl::_impl::from_value<scaled_integer<>>");
    static_assert(identical(scaled_integer<std::uint64_t>{404}, cnl::_impl::from_value<scaled_integer<>>(UINT64_C(404))),
            "cnl::_impl::from_value<scaled_integer<>, cnl::constant<4>>()");

    static_assert(identical(cnl::_impl::from_value<scaled_integer<int32>>(cnl::constant<369>{}), scaled_integer<int>{369}),
            "cnl::_impl::from_value<scaled_integer<>>");
    static_assert(identical(scaled_integer<int, cnl::power<2>>{4}, cnl::_impl::from_value<scaled_integer<>>(cnl::constant<4>{})),
            "cnl::_impl::from_value<scaled_integer<>, cnl::constant<4>>()");
}

namespace test_deduction_guides {
    static_assert(identical(cnl::make_scaled_integer(short{123}), cnl::scaled_integer(short{123})),
            "cnl::scaled_integer class template deduction");
    static_assert(identical(cnl::make_scaled_integer(UINT64_C(404)), cnl::scaled_integer(UINT64_C(404))),
            "cnl::scaled_integer class template deduction");

    static_assert(identical(cnl::make_scaled_integer(cnl::constant<369>{}), cnl::scaled_integer(cnl::constant<369>{})),
            "cnl::scaled_integer class template deduction");

    static_assert(
            identical(
                    scaled_integer<int8>(0),
                    cnl::make_scaled_integer(int8{0})));
    static_assert(identical(cnl::make_scaled_integer(cnl::constant<4>{}), cnl::scaled_integer(cnl::constant<4>{})),
            "cnl::scaled_integer class template deduction");
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::set_digits_t<scaled_integer<>, >

namespace test_set_digits_t {
    using cnl::set_digits_t;

    static_assert(identical(set_digits_t<scaled_integer<uint8, cnl::power<-8>>, 8>{0}, scaled_integer<uint8, cnl::power<-8>>{0}),
                  "cnl::set_digits_t test failed");
    static_assert(is_same<set_digits_t<scaled_integer<int8, cnl::power<8>>, 15>, scaled_integer<int16, cnl::power<8>>>::value,
                  "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint16, 24>, cnl::power<-16>>,
                    set_digits_t<scaled_integer<uint16, cnl::power<-16>>, 24>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int16, 31>, cnl::power<16>>,
                    set_digits_t<scaled_integer<int16, cnl::power<16>>, 31>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint32, 40>, cnl::power<-45>>,
                    set_digits_t<scaled_integer<uint32, cnl::power<-45>>, 40>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int32, 47>, cnl::power<-8>>,
                    set_digits_t<scaled_integer<int32, cnl::power<-8>>, 47>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint64, 56>, cnl::power<8>>,
                    set_digits_t<scaled_integer<uint64, cnl::power<8>>, 56>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int64, 63>, cnl::power<-16>>,
                    set_digits_t<scaled_integer<int64, cnl::power<-16>>, 63>>::value,
            "cnl::set_digits_t test failed");
#if defined(CNL_INT128_ENABLED) || defined(TEST_WIDE_INTEGER)
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint8, 72>, cnl::power<16>>,
                    set_digits_t<scaled_integer<uint8, cnl::power<16>>, 72>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int8, 79>, cnl::power<-45>>,
                    set_digits_t<scaled_integer<int8, cnl::power<-45>>, 79>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint16, 88>, cnl::power<-8>>,
                    set_digits_t<scaled_integer<uint16, cnl::power<-8>>, 88>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int16, 95>, cnl::power<8>>,
                    set_digits_t<scaled_integer<int16, cnl::power<8>>, 95>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint32, 104>, cnl::power<-16>>,
                    set_digits_t<scaled_integer<uint32, cnl::power<-16>>, 104>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int32, 111>, cnl::power<16>>,
                    set_digits_t<scaled_integer<int32, cnl::power<16>>, 111>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<uint64, 120>, cnl::power<-45>>,
                    set_digits_t<scaled_integer<uint64, cnl::power<-45>>, 120>>::value,
            "cnl::set_digits_t test failed");
    static_assert(
            assert_same<
                    scaled_integer<set_digits_t<int64, 127>, cnl::power<-8>>,
                    set_digits_t<scaled_integer<int64, cnl::power<-8>>, 127>>::value,
            "cnl::set_digits_t test failed");
#endif  // defined(CNL_INT128_ENABLED) || defined(TEST_WIDE_INTEGER)

#if defined(CNL_INT128_ENABLED)
#if defined(__GNUC__)
// GCC complains about __int128 with -pedantic or -pedantic-errors
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
    static_assert(
            assert_same<
                    cnl::scaled_integer<set_digits_t<long unsigned int, 96U>, cnl::power<-96>>,
                    cnl::set_digits_t<cnl::scaled_integer<long unsigned int, cnl::power<-96>>, 96U>>::value,
            "");
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#endif  // defined(CNL_INT128_ENABLED)
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::scaled_integer

////////////////////////////////////////////////////////////////////////////////
// default second template parameter

static_assert(
        is_same<scaled_integer<int8>, scaled_integer<int8, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<uint8>, scaled_integer<uint8, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<int16>, scaled_integer<int16, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<uint16>, scaled_integer<uint16, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<int32>, scaled_integer<int32, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<uint32>, scaled_integer<uint32, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<int64>, scaled_integer<int64, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<uint64>, scaled_integer<uint64, cnl::power<0>>>::value,
        "cnl::scaled_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// default first template parameter

static_assert(
        identical(scaled_integer<test_int, cnl::power<0>>{test_int{ 0 }}, scaled_integer<>{test_int{ 0 }}),
        "cnl::scaled_integer test failed");
static_assert(
        is_same<scaled_integer<test_int, cnl::power<0>>, scaled_integer<>>::value,
        "cnl::scaled_integer test failed");

namespace test_glvalue_error {
#if ! defined(__GNUG__) // Bug 71504?
    constexpr auto q = scaled_integer<test_int, cnl::power<0>>{1}/1;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// conversion

// exponent == 0
static_assert(scaled_integer<uint8>(12.34F)==12.F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint16>(12.34F)==12.F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint32>(12.34F)==12.L, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint64>(12.34F)==12.F, "cnl::scaled_integer test failed");

static_assert((scaled_integer<int8>(-12.34F))==-12.F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int16>(-12.34F)==-12.L, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int32>(-12.34F)==-12.F, "cnl::scaled_integer test failed");
static_assert((scaled_integer<int64>(-12.34F))==-12.F, "cnl::scaled_integer test failed");

// exponent = -1
static_assert(scaled_integer<uint8, cnl::power<-1>>(127.5)==127.5, "cnl::scaled_integer test failed");

static_assert(scaled_integer<int8, cnl::power<-1>>(63.5)==63.5, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int8, cnl::power<-1>>(-63.5)==-63.5, "cnl::scaled_integer test failed");

// exponent == -7
static_assert(scaled_integer<uint8, cnl::power<-7>>(.125F)==.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint16, cnl::power<-8>>(232.125F)==232.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint32, cnl::power<-7>>(232.125F)==232.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint64, cnl::power<-7>>(232.125F)==232.125F, "cnl::scaled_integer test failed");

static_assert(scaled_integer<int8, cnl::power<-7>>(.125F)==.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int16, cnl::power<-7>>(123.125F)==123.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int32, cnl::power<-7>>(123.125F)==123.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int64, cnl::power<-7>>(123.125F)==123.125F, "cnl::scaled_integer test failed");

static_assert((scaled_integer<uint8, cnl::power<-7>>(.125F))==.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint16, cnl::power<-8>>(232.125F)==232.125L, "cnl::scaled_integer test failed");
static_assert((scaled_integer<uint32, cnl::power<-7>>(232.125F))==232.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint64, cnl::power<-7>>(232.125F)==232.125L, "cnl::scaled_integer test failed");

#if !defined(TEST_THROWING_OVERFLOW_INTEGER) \
        && !defined(TEST_TRAPPING_OVERFLOW_INTEGER) \
        && !defined(TEST_UNDEFINED_OVERFLOW_INTEGER) \
        && !defined(TEST_WIDE_INTEGER_INT) \
        && !defined(TEST_WIDE_INTEGER_32)
static_assert(scaled_integer<int8, cnl::power<-7>>(1)!=1.L, "cnl::scaled_integer test failed");
#endif

#if !defined(TEST_THROWING_OVERFLOW_INTEGER) \
        && !defined(TEST_TRAPPING_OVERFLOW_INTEGER) \
        && !defined(TEST_UNDEFINED_OVERFLOW_INTEGER) \
        && !defined(TEST_SATURATED_OVERFLOW_INTEGER) \
        && !defined(TEST_WIDE_INTEGER_INT) \
        && !defined(TEST_WIDE_INTEGER_32)
static_assert(scaled_integer<int8, cnl::power<-7>>(1)==-1.L, "cnl::scaled_integer test failed");
#endif

static_assert(scaled_integer<int8, cnl::power<-7>>(.5)==.5F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int8, cnl::power<-7>>(.125F)==.125L, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int16, cnl::power<-7>>(123.125F)==123.125F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int32, cnl::power<-7>>(123.125F)==123.125, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int64, cnl::power<-7>>(123.125L)==123.125F, "cnl::scaled_integer test failed");

// exponent == 16
#if !defined(TEST_WIDE_INTEGER_8)
static_assert(scaled_integer<uint8, cnl::power<16>>(test_int{ 65536 }) == 65536.F, "cnl::scaled_integer test failed");
#endif
#if defined(TEST_WIDE_INTEGER_INT) || defined(TEST_WIDE_INTEGER_32)
static_assert(scaled_integer<uint16, cnl::power<16>>(6553.)==0U, "cnl::scaled_integer test failed");
#else
static_assert(scaled_integer<uint16, cnl::power<16>>(6553.)==0, "cnl::scaled_integer test failed");
#endif
static_assert((scaled_integer<uint32, cnl::power<16>>(4294967296L))==4294967296.F, "cnl::scaled_integer test failed");
#if defined(CNL_INT128_ENABLED)
static_assert((scaled_integer<uint64, cnl::power<16>>(1125895611875328L))==1125895611875328UL, "cnl::scaled_integer test failed");
#endif

static_assert(scaled_integer<int8, cnl::power<16>>(-65536)==-65536.F, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int16, cnl::power<16>>(-6553.)==0, "cnl::scaled_integer test failed");
static_assert((scaled_integer<int32, cnl::power<16>>(-4294967296L))==-4294967296.F, "cnl::scaled_integer test failed");
static_assert((scaled_integer<int64, cnl::power<16>>(-0x800000000000LL))==-0x800000000000LL, "cnl::scaled_integer test failed");

// exponent = 1
#if defined(TEST_WIDE_INTEGER_INT) || defined(TEST_WIDE_INTEGER_32)
static_assert(scaled_integer<uint8, cnl::power<1>>(10)==10U, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint8, cnl::power<1>>(11)==10U, "cnl::scaled_integer test failed");
#else
static_assert(scaled_integer<uint8, cnl::power<1>>(10)==10, "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint8, cnl::power<1>>(11)==10, "cnl::scaled_integer test failed");
#endif
static_assert(scaled_integer<int8, cnl::power<1>>(123.5)==122, "cnl::scaled_integer test failed");

static_assert(scaled_integer<int8, cnl::power<1>>(127)==126, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int8, cnl::power<1>>(126)==126, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int8, cnl::power<1>>(-5)==-4, "cnl::scaled_integer test failed");

// conversion between scaled_integer specializations
static_assert(scaled_integer<uint8, cnl::power<-4>>(scaled_integer<int16, cnl::power<-8>>(1.5))==1.5, "cnl::scaled_integer test failed");
#if !defined(TEST_WIDE_INTEGER_8)
static_assert(scaled_integer<uint16, cnl::power<-8>>(scaled_integer<int8, cnl::power<-4>>(3.25))==3.25, "cnl::scaled_integer test failed");
#endif
static_assert(
        identical(
                scaled_integer<uint8, cnl::power<4>>{768},
                scaled_integer<uint8, cnl::power<4>>(scaled_integer<int16, cnl::power<-4>>(768))),
        "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint32, cnl::power<-24>>(scaled_integer<uint64, cnl::power<-48>>(3.141592654))>3.1415923F,
        "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint32, cnl::power<-24>>(scaled_integer<uint64, cnl::power<-48>>(3.141592654))<3.1415927F,
        "cnl::scaled_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(scaled_integer<>(-1), "cnl::scaled_integer test failed");
static_assert(scaled_integer<>(1024), "cnl::scaled_integer test failed");
static_assert(!scaled_integer<>(0), "cnl::scaled_integer test failed");

////////////////////////////////////////////////////////////////////////////////
// fixed-point arithmetic support types

namespace test_arithmetic {
    using cnl::_impl::add_op;
    using cnl::_impl::multiply_op;
    using cnl::_impl::divide_op;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_scaled_integer_impl::rep_op_exponent

    static_assert(
            identical(
                    cnl::binary_operator<
                            cnl::_impl::subtract_op,
                            cnl::_impl::native_tag, cnl::_impl::native_tag,
                            scaled_integer<int32>, cnl::constant<369>>()(
                                    scaled_integer<int32>{999}, cnl::constant<369>{}),
                                    scaled_integer<test_int, cnl::power<0>>{630}),
            "cnl::_scaled_integer_impl::rep_op_exponent test failed");
}

////////////////////////////////////////////////////////////////////////////////
// comparison

// These tests pass but produce a signed/unsigned comparison warning as a side effect.
#if !defined(TEST_WIDE_INTEGER)
// heterogeneous fixed-point to fixed-point comparison
static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)==scaled_integer<int16, cnl::power<-7>>(4.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint8, cnl::power<-4>>(4.5)==scaled_integer<int16, cnl::power<-7>>(-4.5)), "cnl::scaled_integer test failed");

static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)!=scaled_integer<int16, cnl::power<-7>>(-4.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint8, cnl::power<-4>>(4.5)!=scaled_integer<int16, cnl::power<-7>>(4.5)), "cnl::scaled_integer test failed");

static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)<scaled_integer<int16, cnl::power<-7>>(5.6), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<int8, cnl::power<-3>>(-4.5)<scaled_integer<int16, cnl::power<-7>>(-5.6)), "cnl::scaled_integer test failed");

static_assert(scaled_integer<uint8, cnl::power<-4>>(4.6)>scaled_integer<int16, cnl::power<-8>>(.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint8, cnl::power<-4>>(4.6)<scaled_integer<int16, cnl::power<-8>>(-4.5)), "cnl::scaled_integer test failed");

static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)<=scaled_integer<int16, cnl::power<-8>>(4.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint8, cnl::power<-4>>(4.5)<=scaled_integer<int16, cnl::power<-8>>(-4.5)), "cnl::scaled_integer test failed");

static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)>=scaled_integer<int16, cnl::power<-8>>(.5), "cnl::scaled_integer test failed");
static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)>=scaled_integer<int16, cnl::power<-8>>(-4.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint8, cnl::power<-4>>(4.5)>=scaled_integer<int16, cnl::power<-8>>(4.6)), "cnl::scaled_integer test failed");
#endif

// heterogeneous fixed-point to arithmetic comparison
static_assert(scaled_integer<uint8, cnl::power<-4>>(4.5)==4.5, "cnl::scaled_integer test failed");
static_assert(!(4==scaled_integer<int16, cnl::power<-7>>(-4.5)), "cnl::scaled_integer test failed");

static_assert(4.5F!=scaled_integer<int16, cnl::power<-7>>(-4.5), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<uint64, cnl::power<-4>>(4.5)!=4.5L), "cnl::scaled_integer test failed");

static_assert(4.5<scaled_integer<int16, cnl::power<-7>>(5.6), "cnl::scaled_integer test failed");
static_assert(!(scaled_integer<int32, cnl::power<-3>>(-4.5)<-5.6), "cnl::scaled_integer test failed");

static_assert(scaled_integer<int16, cnl::power<-13>>(3.141) > 3, "cnl::scaled_integer test failed");
static_assert(scaled_integer<int32, cnl::power<-15>>{-0.2} < 0, "cnl::scaled_integer test failed");

#if !defined(TEST_WIDE_INTEGER_8)
static_assert(scaled_integer<uint8, cnl::power<4>>(768)==uint8{768/16}*uint8{16}, "cnl::scaled_integer test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert((scaled_integer<int32>(123)+scaled_integer<int32>(123))==246, "cnl::scaled_integer addition operator test failed");
static_assert((scaled_integer<int32, cnl::power<-16>>(123.125)+scaled_integer<int32, cnl::power<-16>>(123.75))==246.875, "cnl::scaled_integer addition operator test failed");
static_assert((scaled_integer<int32, cnl::power<-16>>(123.125)+scaled_integer<int32, cnl::power<-16>>(123.75))==246.875, "cnl::scaled_integer addition operator test failed");

static_assert(
        identical(
                scaled_integer<decltype(std::declval<int8>()+std::declval<int8>()), cnl::power<-5>>{-1.125F},
                scaled_integer<int8, cnl::power<-5>>{2.125}+scaled_integer<int8, cnl::power<-5>>{-3.25}),
        "cnl::scaled_integer addition operator test failed");

static_assert(
        identical(
                scaled_integer<decltype(std::declval<uint8>()+std::declval<test_int>()), cnl::power<-4>>{2049.5},
                scaled_integer<uint8, cnl::power<-4>>{1.5}+2048),
        "test failed");
static_assert(
        identical(
                scaled_integer<decltype(std::declval<test_signed>()+std::declval<uint8>()), cnl::power<-4>>{2050.25},
                scaled_integer<>{2048}+scaled_integer<uint8, cnl::power<-4>>{2.25}),
        "cnl::scaled_integer addition operator test failed");
#if !defined(TEST_WIDE_INTEGER_8)
static_assert(
        identical(
                scaled_integer<
                        decltype(std::declval<test_int>()+std::declval<uint8>()*std::declval<test_int>()),
                        cnl::power<0>>{12288},
                2048+scaled_integer<uint8, cnl::power<10>>(10240)),
        "cnl::scaled_integer addition operator test failed");
#endif
static_assert(identical(16777981.428100586F, 765.432F+scaled_integer<int64, cnl::power<-32>>(16777215.996093750)),
        "cnl::scaled_integer addition operator test failed");
static_assert(identical(16777981.42809375, 765.432+scaled_integer<int64, cnl::power<-32>>(16777215.996093750)),
        "cnl::scaled_integer addition operator test failed");
static_assert(identical(16777981.428100586F, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)+765.432F),
        "cnl::scaled_integer addition operator test failed");
static_assert(identical(16777981.42809375, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)+765.432),
        "cnl::scaled_integer addition operator test failed");

static_assert(identical(scaled_integer<int32, cnl::power<-16>>{.5} + 2, scaled_integer<test_int, cnl::power<-16>>{2.5}),
        "cnl::scaled_integer addition operator test failed");

// subtraction
static_assert((scaled_integer<int32>(999)-scaled_integer<int32>(369))==630, "cnl::scaled_integer subtraction test failed");
static_assert((scaled_integer<int32, cnl::power<-16>>(246.875)-scaled_integer<int32, cnl::power<-16>>(123.75))==123.125, "cnl::scaled_integer test failed");
static_assert((scaled_integer<int16, cnl::power<-4>>(123.125)-scaled_integer<int16, cnl::power<-4>>(246.875))==-123.75, "cnl::scaled_integer test failed");

static_assert(scaled_integer<int8, cnl::power<-5>>(2.125)-scaled_integer<int8, cnl::power<-5>>(3.25)==-1.125F, "cnl::scaled_integer subtraction test failed");
static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                scaled_integer<decltype(std::declval<int8>()-std::declval<int8>()), cnl::power<-5>>(2.125-3.25),
                scaled_integer<int8, cnl::power<-5>>(2.125)-scaled_integer<int8, cnl::power<-5>>(3.25)),
        "cnl::scaled_integer subtraction test failed");
static_assert(
        identical(
                scaled_integer<decltype(std::declval<unsigned>()-std::declval<uint8>()), cnl::power<-3>>{2048U-0.875},
                2048U-scaled_integer<uint8, cnl::power<-3>>(0.875)),
        "cnl::scaled_integer subtraction test failed");
static_assert(
        identical(
                scaled_integer<
                        // NOLINTNEXTLINE(misc-redundant-expression)
                        decltype(std::declval<test_signed>()-std::declval<test_int>()),
                        cnl::power<-3>>{0.875-2048},
                scaled_integer<int8, cnl::power<-3>>(0.875)-2048),
        "cnl::scaled_integer subtraction test failed");
#if !defined(TEST_WIDE_INTEGER_8)
static_assert(
        identical(
                scaled_integer<decltype(std::declval<uint8>()-std::declval<int>()), cnl::power<0>>{10240-2048},
                scaled_integer<uint8, cnl::power<10>>(10240)-2048),
        "cnl::scaled_integer subtraction test failed");
#endif
static_assert(identical(-16776450.564086914F, 765.432F-scaled_integer<int64, cnl::power<-32>>(16777215.996093750)),
        "cnl::scaled_integer subtraction test failed");

// multiplication
static_assert(
        identical(
                scaled_integer<decltype(std::declval<uint8>()*std::declval<uint8>()), cnl::power<0>>{0xaa},
                scaled_integer<uint8>{0x55}*scaled_integer<uint8>{2}),
        "cnl::scaled_integer test failed");

static_assert(
        identical(
                scaled_integer<int64, cnl::power<-16>>{123.75} * scaled_integer<int32, cnl::power<-16>>(44.5),
                scaled_integer<int64, cnl::power<-32>>{5506.875}),
        "cnl::scaled_integer test failed");

#if !defined(TEST_WIDE_INTEGER_8)
static_assert(
        identical(
                scaled_integer<decltype(std::declval<int8>()*std::declval<int8>()), cnl::power<-10>>{-6.90625},
                scaled_integer<int8, cnl::power<-5>>{2.125}*scaled_integer<int8, cnl::power<-5>>{-3.25}),
        "cnl::scaled_integer multiplication test failed");
#endif

#if !defined(TEST_WIDE_INTEGER)
static_assert(
        identical(
                scaled_integer<uint8, cnl::power<10>>{10240} * 3U,
                scaled_integer<test_unsigned, cnl::power<10>>{30720}),
        "cnl::scaled_integer multiplication test failed");

static_assert(identical(3U * uint8{4}, test_unsigned{12}), "cnl::scaled_integer multiplication test failed");
static_assert(
        identical(
                3U * scaled_integer<uint8, cnl::power<10>>{10240},
                scaled_integer<test_unsigned, cnl::power<10>>{30720}),
        "cnl::scaled_integer multiplication test failed");
#endif

static_assert(identical(-2074569855.5169766F, -123.654F*scaled_integer<int64, cnl::power<-32>>(16777215.996093750)),
        "cnl::scaled_integer multiplication test failed");
static_assert(identical(12841817994.32203125F, 765.432F*scaled_integer<int64, cnl::power<-32>>(16777215.996093750)),
        "cnl::scaled_integer multiplication test failed");
static_assert(identical(-2074569855.5169766F, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)*-123.654F),
        "cnl::scaled_integer multiplication test failed");
static_assert(identical(-2074569866.7809765625, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)*-123.654),
        "cnl::scaled_integer multiplication test failed");

// division
static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                scaled_integer<decltype(std::declval<int8>()/std::declval<int8>()), cnl::power<0>>{-15.75},
                scaled_integer<int8, cnl::power<-1>>{63}/scaled_integer<int8, cnl::power<-1>>{-4}),
        "cnl::scaled_integer test failed");
static_assert(
        identical(
                scaled_integer<test_int, cnl::power<-1>>{63}/scaled_integer<int8, cnl::power<-1>>{-4},
                scaled_integer<test_int, cnl::power<0>>{-15.75}),
        "cnl::scaled_integer test failed");
static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                scaled_integer<decltype(std::declval<int8>()/std::declval<int8>()), cnl::power<0>>{31.75},
                scaled_integer<int8, cnl::power<1>>{-255}/scaled_integer<int8, cnl::power<1>>{-8}),
        "cnl::scaled_integer test failed");
static_assert(
        (scaled_integer<int8, cnl::power<1>>(-255)/scaled_integer<int8, cnl::power<1>>(-8))==31,
        "cnl::scaled_integer test failed");

static_assert(
        identical(
                // NOLINTNEXTLINE(misc-redundant-expression)
                scaled_integer<decltype(std::declval<int8>()/std::declval<int8>()), cnl::power<0>>{0},
                scaled_integer<int8, cnl::power<-5>>{2.5}/scaled_integer<int8, cnl::power<-5>>{-4.F}),
        "cnl::scaled_integer division test failed");

static_assert(
        identical(
                scaled_integer<decltype(std::declval<test_int>()/std::declval<uint8>()), cnl::power<6>>{40},
                test_int{10}/scaled_integer<uint8, cnl::power<-6>>(0.25)),
        "cnl::scaled_integer division test failed");
static_assert(
        identical(
                scaled_integer<decltype(std::declval<uint8>()/std::declval<uint16>()), cnl::power<10>>{3413.3333333},
                scaled_integer<uint8, cnl::power<10>>{10240}/static_cast<uint16>(3)),
        "cnl::scaled_integer division test failed");

static_assert(
        identical(
                scaled_integer<decltype(std::declval<test_int>()/std::declval<uint8>()), cnl::power<2>>{40},
                test_int{10}/scaled_integer<uint8, cnl::power<-2>>{0.25}),
        "cnl::scaled_integer division test failed");

static_assert(16777215.996093750/scaled_integer<int64, cnl::power<-32>>(-123.654F)==-135678.71712347874,
        "cnl::scaled_integer division test failed");
static_assert(is_same<decltype(16777215.996093750/scaled_integer<int64, cnl::power<-32>>(-123.654F)), double>::value,
        "cnl::scaled_integer division test failed");
static_assert(identical(-135678.71712347874F, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)/-123.654F),
        "cnl::scaled_integer division test failed");
static_assert(identical(-135678.7163868031, scaled_integer<int64, cnl::power<-32>>(16777215.996093750)/-123.654),
        "cnl::scaled_integer division test failed");

static_assert(
        identical(
                scaled_integer<uint32, cnl::power<0>>{0xffffffff}/scaled_integer<uint32, cnl::power<0>>{0xffffffff},
                scaled_integer<uint32, cnl::power<0>>{1}),
        "cnl::scaled_integer test failed");
#if defined(CNL_INT128_ENABLED)
static_assert(cnl::numeric_limits<uint128>::is_specialized, "");
static_assert(cnl::numeric_limits<uint128>::is_integer, "");
#endif

namespace test_shift_operator_right {
    constexpr auto expected{cnl::scaled_integer<test_int, cnl::power<-28> >{1.28125}};
    constexpr auto lhs{cnl::scaled_integer<test_int, cnl::power<-28> >{5.125}};
    constexpr auto rhs{test_int{2}};
    constexpr auto op{
            cnl::shift_operator<
                    cnl::_impl::shift_right_op,
                    cnl::_impl::native_tag, cnl::_impl::native_tag,
                    cnl::scaled_integer<test_int, cnl::power<-28> >,
                    test_int>{}
    };
    constexpr auto actual{op(lhs, rhs)};
    static_assert(identical(expected, actual), "");
}

#if !defined(__clang__) || (__clang_major__ > 3) || (__clang_minor__ > 8)
namespace test_shift_operator_left {
    constexpr auto expected{
            scaled_integer<
                    decltype(std::declval<uint8>() << std::declval<test_int>()),
                    cnl::power<-4>>{2}};
    constexpr auto lhs{scaled_integer<uint8, cnl::power<-4>>{1}};
    constexpr auto rhs{scaled_integer<>{1}};
    constexpr auto op{
            cnl::shift_operator<
                    cnl::_impl::shift_left_op,
                    cnl::_impl::native_tag, cnl::_impl::native_tag,
                    scaled_integer<uint8, cnl::power<-4>>,
                    scaled_integer<>>{}
    };
    constexpr auto actual{op(lhs, rhs)};
    static_assert(identical(expected, actual), "");
}
#endif

namespace test_bitshift {
    // dynamic
#if !defined(__clang__) || (__clang_major__ > 3) || (__clang_minor__ > 8)
    static_assert(
            identical(
                    scaled_integer<decltype(std::declval<uint8>() << std::declval<test_int>()), cnl::power<-4>>{2},
                    scaled_integer<uint8, cnl::power<-4>>{1} << 1), "bitshift test failed");
#endif
    static_assert(
            identical(
                    scaled_integer<decltype(std::declval<uint8>() >> std::declval<test_int>()), cnl::power<-4>>{.5},
                    scaled_integer<uint8, cnl::power<-4>>{1} >> 1), "bitshift test failed");
#if !defined(__clang__) || (__clang_major__ > 3) || (__clang_minor__ > 9)
    static_assert(
            identical(
                    scaled_integer<decltype(std::declval<uint8>() << std::declval<test_int>()), cnl::power<-4>>{2},
                    scaled_integer<uint8, cnl::power<-4>>{1} << scaled_integer<>{1}), "bitshift test failed");
#endif

    // cnl::constant
    static_assert(
            identical(
                    scaled_integer<uint8, cnl::power<-3>>{2},
                    scaled_integer<uint8, cnl::power<-4>>{1} << cnl::constant<1>{}), "bitshift test failed");

    static_assert(
            identical(
                    scaled_integer<uint8, cnl::power<-5>>{.5},
                    scaled_integer<uint8, cnl::power<-4>>{1} >> cnl::constant<1>{}), "bitshift test failed");

    // const_integer
    using namespace cnl::literals;
    static_assert(
            identical(
                    scaled_integer<uint8, cnl::power<-3>>{2},
                    scaled_integer<uint8, cnl::power<-4>>{1} << 1_c), "bitshift test failed");

    static_assert(
            identical(
                    scaled_integer<uint8, cnl::power<-5>>{.5},
                    scaled_integer<uint8, cnl::power<-4>>{1} >> 1_c), "bitshift test failed");
}

namespace test_bitwise_or {
    static_assert(identical(
            scaled_integer<test_unsigned, cnl::power<-16>>{125.625},
            scaled_integer<test_unsigned, cnl::power<-12>>{125.125} | scaled_integer<test_unsigned, cnl::power<-16>>{77.625}),
                  "operator|(scaled_integer<>,scaled_integer<>)");
}

namespace test_bitwise_and {
    static_assert(identical(
            scaled_integer<test_unsigned, cnl::power<-16>>{77.125},
            scaled_integer<test_unsigned, cnl::power<-12>>{125.125} & scaled_integer<test_unsigned, cnl::power<-16>>{77.625}),
            "operator&(scaled_integer<>,scaled_integer<>)");
}

namespace test_bitwise_xor {
    static_assert(identical(
            scaled_integer<test_unsigned, cnl::power<-16>>{48.5},
            scaled_integer<test_unsigned, cnl::power<-12>>{125.125}
                    ^ scaled_integer<test_unsigned, cnl::power<-16>>{77.625}),
            "operator^(scaled_integer<>,scaled_integer<>)");
}

////////////////////////////////////////////////////////////////////////////////
// quotient

static_assert(cnl::numeric_limits<uint8>::max()/5==51, "");
static_assert(cnl::numeric_limits<uint8>::max()/3==85, "");

namespace test_quotient {
#if !defined(__clang__) || (__clang_major__ > 3) || (__clang_minor__ > 8)
    static_assert(
            identical(
                    scaled_integer<quot_digits_t<test_int, int16>, cnl::power<-29>>{1./127},
                    cnl::quotient(scaled_integer<test_int, cnl::power<-14>>{1}, int16{127})),
            "cnl::quotient test failed");
    static_assert(
            identical(
                    cnl::quotient(int32(-999), int32(3)),
                    scaled_integer<quot_digits_t<>, cnl::power<-31>>{-333LL}),
            "cnl::scaled_integer test failed");
    static_assert(
            identical(
                    cnl::quotient(scaled_integer<uint32, cnl::power<10>>{10240}, uint32{3u}),
                    scaled_integer<uint64, cnl::power<-22>>{3413.3333333}),
            "cnl::scaled_integer division test failed");
    static_assert(
            identical(
                    scaled_integer<quot_digits_t<test_int, uint8>, cnl::power<-6>>{40},
                    cnl::quotient(test_int{10}, scaled_integer<uint8, cnl::power<-2>>{0.25})),
            "cnl::scaled_integer division test failed");
#if defined(CNL_INT128_ENABLED)
    static_assert(
            identical(
                    cnl::quotient(
                            scaled_integer<uint64, cnl::power<0>>{0xFFFFFFFE00000001LL},
                            scaled_integer<uint64, cnl::power<-32>>{0xffffffffULL}),
                    scaled_integer<uint128, cnl::power<-32>>{0xffffffffULL}),
            "cnl::scaled_integer test failed");
#endif
    static_assert(
            identical(
                    cnl::quotient(
                            scaled_integer<uint32, cnl::power<0>>{0xFFFE0001LL},
                            scaled_integer<uint32, cnl::power<0>>{0xffff}),
                    scaled_integer<uint64, cnl::power<-32>>{0xffffLL}),
            "cnl::scaled_integer test failed");
    static_assert(
            identical(
                    scaled_integer<cnl::set_digits_t<int32, cnl::digits<int32>::value+2>, cnl::power<-18>>{321LL},
                    cnl::quotient(cnl::scaled_integer<int32, cnl::power<-16>>{963}, cnl::constant<3>{})),
            "cnl::scaled_integer test failed");
#endif
}

////////////////////////////////////////////////////////////////////////////////
// cnl::numeric_limits<scaled_integer<>>

template<class Rep, int Exponent, class Min, class Max, class Lowest>
CNL_NODISCARD constexpr bool test_numeric_limits(Min min, Max max, Lowest lowest)
{
    using fp = scaled_integer<Rep, cnl::power<Exponent>>;
    using nl = cnl::numeric_limits<fp>;
    using rnl = cnl::numeric_limits<Rep>;

    static_assert(cnl::numeric_limits<Rep>::is_specialized,
                  "cnl::numeric_limits<Rep>::is_specialized");
    static_assert(cnl::numeric_limits<Rep>::is_integer,
                  "cnl::numeric_limits<Rep>::is_integer");

    static_assert(cnl::numeric_limits<fp>::is_specialized,
                  "cnl::numeric_limits<scaled_integer<Rep>>::is_specialized");

    static_assert(nl::is_specialized, "numeric_limits<scaled_integer>::is_specialized");
    static_assert(nl::is_signed==rnl::is_signed, "numeric_limits<scaled_integer>::is_signed");
    static_assert(!nl::is_integer, "numeric_limits<scaled_integer>::is_integer");
    static_assert(nl::is_exact, "numeric_limits<scaled_integer>::is_exact");
    static_assert(!nl::has_infinity, "numeric_limits<scaled_integer>::has_infinity");
    static_assert(!nl::has_quiet_NaN, "numeric_limits<scaled_integer>::has_quiet_NaN");
    static_assert(!nl::has_signaling_NaN, "numeric_limits<scaled_integer>::has_signaling_NaN");
    static_assert(!nl::has_denorm, "numeric_limits<scaled_integer>::has_denorm");
    static_assert(!nl::has_denorm_loss, "numeric_limits<scaled_integer>::has_denorm_loss");
    static_assert(nl::round_style==std::round_toward_zero, "numeric_limits<scaled_integer>::round_style");
    static_assert(!nl::is_iec559, "numeric_limits<scaled_integer>::is_iec559");
    static_assert(nl::is_bounded, "numeric_limits<scaled_integer>::is_bounded");
    static_assert(nl::is_modulo==rnl::is_modulo, "numeric_limits<scaled_integer>::is_modulo");
    static_assert(nl::digits==rnl::digits, "numeric_limits<scaled_integer>::digits");
    static_assert(nl::digits10==rnl::digits10, "numeric_limits<scaled_integer>::digits10");
    static_assert(nl::max_digits10==rnl::max_digits10, "numeric_limits<scaled_integer>::max_digits10");
    static_assert(nl::radix==2, "numeric_limits<scaled_integer>::radix");
    static_assert(nl::min_exponent==rnl::min_exponent, "numeric_limits<scaled_integer>::min_exponent");
    static_assert(nl::min_exponent10==rnl::min_exponent10, "numeric_limits<scaled_integer>::min_exponent10");
    static_assert(nl::max_exponent==rnl::max_exponent, "numeric_limits<scaled_integer>::max_exponent");
    static_assert(nl::max_exponent10==rnl::max_exponent10, "numeric_limits<scaled_integer>::max_exponent10");
    static_assert(nl::traps==rnl::traps, "numeric_limits<scaled_integer>::traps");
    static_assert(!nl::tinyness_before, "numeric_limits<scaled_integer>::tinyness_before");
    static_assert(nl::round_error()==static_cast<Rep>(0), "numeric_limits<scaled_integer>::round_error");
    static_assert(nl::infinity()==rnl::infinity(), "numeric_limits<scaled_integer>::infinity");
    static_assert(nl::quiet_NaN()==Rep(0), "numeric_limits<scaled_integer>::quiet_NaN");
    static_assert(nl::signaling_NaN()== Rep(0), "numeric_limits<scaled_integer>::signaling_NaN");

    return nl::min()==min
            && nl::lowest()==lowest
            && nl::max()==max
            && nl::epsilon()==min
            && nl::denorm_min()==min;
}

static_assert(cnl::numeric_limits<scaled_integer<test_int, cnl::power<-256>>>::lowest() < -.1e-67, "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_int, cnl::power<-256>>>::min() > 0., "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_int, cnl::power<-256>>>::min() < .1e-76, "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_int, cnl::power<-256>>>::max() > .1e-67, "cnl::numeric_limits<scaled_integer> test failed");

static_assert(cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<-256>>>::lowest() == 0., "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<-256>>>::min() > 0., "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<-256>>>::min() < .1e-76, "cnl::numeric_limits<scaled_integer> test failed");
static_assert(cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<-256>>>::max() > .1e-67, "cnl::numeric_limits<scaled_integer> test failed");

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

static_assert(
        cnl::numeric_limits<scaled_integer<test_int, cnl::power<256>>>::lowest() < -1.e86,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_int, cnl::power<256>>>::min() > 1.e77,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_int, cnl::power<256>>>::min() < 1.e78,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_int, cnl::power<256>>>::max() > 1.e86,
        "cnl::numeric_limits<scaled_integer> test failed");

static_assert(
        cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<256>>>::lowest() == 0.,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<256>>>::min() > 1.e77,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<256>>>::min() < 1.e78,
        "cnl::numeric_limits<scaled_integer> test failed");
static_assert(
        cnl::numeric_limits<scaled_integer<test_unsigned, cnl::power<256>>>::max() > 1.e86,
        "cnl::numeric_limits<scaled_integer> test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::sqrt

#if !defined(TEST_WIDE_INTEGER_INT) && !defined(TEST_WIDE_INTEGER_32)
static_assert(sqrt(scaled_integer<uint8>(225))==15, "cnl::sqrt test failed");
static_assert(sqrt(scaled_integer<int8>(81))==9, "cnl::sqrt test failed");

static_assert(sqrt(scaled_integer<uint8, cnl::power<-1>>(4))==2, "cnl::sqrt test failed");
static_assert(sqrt(scaled_integer<int, cnl::power<-2>>(9))==3, "cnl::sqrt test failed");
static_assert(sqrt(scaled_integer<int8, cnl::power<-2>>(9))==3, "cnl::sqrt test failed");

static_assert(sqrt(scaled_integer<uint8, cnl::power<-4>>(4))==2, "cnl::sqrt test failed");
static_assert(
        static_cast<float>(sqrt(scaled_integer<int32, cnl::power<-24>>(3.141592654)))>1.7724537849426,
        "cnl::sqrt test failed");
static_assert(
        static_cast<float>(sqrt(scaled_integer<int32, cnl::power<-24>>(3.141592654)))<1.7724537849427,
        "cnl::sqrt test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// std::leading_bits<scaled_integer>

namespace test_used_digits {
    static_assert(cnl::used_digits(scaled_integer<uint8, cnl::power<-4>>{1}) == 5, "cnl::used_digits");
    static_assert(cnl::used_digits(scaled_integer<uint8, cnl::power<-3>>{2}) == 5, "cnl::used_digits");
}

namespace test_leading_bits {
    using cnl::leading_bits;

    static_assert(leading_bits(scaled_integer<uint8, cnl::power<-4>>{1}) == 3, "cnl::leading_bits");
    static_assert(leading_bits(scaled_integer<uint8, cnl::power<-3>>{2}) == 3, "cnl::leading_bits");
    static_assert(leading_bits(scaled_integer<int32, cnl::power<10>>{4096}) == 28, "cnl::leading_bits");
}

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/cDZIcB1LNfE/heaucUIAAgAJ

namespace test_natural_arithmetic {
    static constexpr scaled_integer<int32, cnl::power<-16>> x{1.5};
    static constexpr auto y = 6.5*x-4;
    static_assert(y==5.75, "usage test failed");
}

////////////////////////////////////////////////////////////////////////////////
// ScaledIntegerTester

template <class Rep, int Exponent>
struct ScaledIntegerTesterOutsize {
    using scaled_integer = ::scaled_integer<Rep, cnl::power<Exponent>>;

    // Rep
    using rep = Rep;
    static_assert(sizeof(rep) == sizeof(scaled_integer), "scaled_integer must be the same size as its Rep");
    static_assert(
            is_same<rep, cnl::_impl::rep_t<scaled_integer>>::value,
            "mismatched rep");    // possibly overly restrictive (e.g. hardware-specific specializations)

    // Exponent
    static constexpr int exponent = Exponent;
    static_assert(
            exponent==cnl::_impl::tag_t<scaled_integer>::exponent,
            "mismatched exponent");

    // simply assignment to and from underlying representation
    using numeric_limits = cnl::numeric_limits<scaled_integer>;
    static constexpr scaled_integer min = cnl::_impl::from_rep<scaled_integer>(rep(1));
#if !defined(_MSC_VER)
    static_assert(cnl::_impl::to_rep(min) == rep(1), "all Rep types should be able to store the number 1!");
#endif

    // unary common_type_t
    static_assert(is_same<
                    cnl::_impl::common_type_t<scaled_integer>,
                    ::scaled_integer<
                            typename std::common_type<Rep>::type,
                            cnl::power<Exponent>>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    static_assert(
            is_same<
                    scaled_integer,
                    cnl::_impl::common_type_t<scaled_integer>>::value,
            "... and that rule should be to do nothing very exciting at all");

    // binary common_type_t
    static_assert(
            is_same<
                    scaled_integer,
                    cnl::_impl::common_type_t<scaled_integer, scaled_integer>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    // for convenience, scaled_integer API assumes binary and unary homogeneous common_types are the same
    static_assert(
            is_same<
                    cnl::_impl::common_type_t<scaled_integer>,
                    cnl::_impl::common_type_t<scaled_integer, scaled_integer>>::value,
            "bad assumption about binary cnl::_impl::common_type_t");

    // test promotion rules for arithmetic
    static_assert(
            is_same<
                    decltype(min + min),
                    ::scaled_integer<decltype(declval<rep>() + declval<rep>()), cnl::power<exponent>>>::value,
            "promotion rule for addition scaled_integer<Rep> should match its Rep");
    static_assert(
            is_same<
                    decltype(min - min),  // NOLINT(misc-redundant-expression)
                    ::scaled_integer<decltype(declval<rep>() - declval<rep>()), cnl::power<exponent>>>::value,
            "promotion rule for subtraction scaled_integer<Rep> should match its Rep");
};

// some tests cannot be performed at run-time because
// they involve bit-shifts that exceed the capacity of the underlying type
template<class Rep, int Exponent>
struct ScaledIntegerTester : public ScaledIntegerTesterOutsize<Rep, Exponent> {
    using super = ScaledIntegerTesterOutsize<Rep, Exponent>;
    using scaled_integer = typename super::scaled_integer;
    static constexpr scaled_integer min = super::min;

#if defined(__clang__) && defined(TEST_NATIVE_INTEGER)
    // assorted tests of +, -, * and /
    static_assert(min + min == 2 * min, "basic arithmetic isn't working");
    static_assert(84 * min / 84 == min, "basic arithmetic isn't working");
    static_assert((7 * min) - (4 * min) == 6 * min / 2, "basic arithmetic isn't working");
#endif
};

template <typename Rep>
struct ScaledIntegerRepTester {
    ScaledIntegerTesterOutsize<Rep, -100> _0;
    ScaledIntegerTester<Rep, -10> _1;
    ScaledIntegerTester<Rep, -1> _2;
    ScaledIntegerTester<Rep, 0> _3;
    ScaledIntegerTester<Rep, 1> _4;
    ScaledIntegerTester<Rep, 10> _5;
    ScaledIntegerTesterOutsize<Rep, 100> _6;

    // test deduction guides
    static_assert(identical(cnl::scaled_integer{Rep{0}}, cnl::scaled_integer<Rep, cnl::power<0>>{0}));
    static_assert(identical(cnl::scaled_integer(Rep{0}), cnl::scaled_integer<Rep, cnl::power<0>>(0)));

    static_assert(identical(cnl::make_scaled_integer(Rep{0}), cnl::scaled_integer(Rep{0})));
};

template struct ScaledIntegerRepTester<int8>;
template struct ScaledIntegerRepTester<uint8>;

template struct ScaledIntegerRepTester<int16>;
template struct ScaledIntegerRepTester<uint16>;

template struct ScaledIntegerRepTester<int32>;
template struct ScaledIntegerRepTester<uint32>;

template struct ScaledIntegerRepTester<int64>;
template struct ScaledIntegerRepTester<uint64>;

#endif
