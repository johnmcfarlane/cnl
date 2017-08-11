
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of fixed_point with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > cnl::width(test_int)::value).

#include <gtest/gtest.h>

////////////////////////////////////////////////////////////////////////////////
// test name generation

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

////////////////////////////////////////////////////////////////////////////////
// imports from std

using std::is_same;
using std::declval;
using std::numeric_limits;

////////////////////////////////////////////////////////////////////////////////
// integer definitions
//
// depends upon test_signed and test_unsigned defined in including source file

static_assert(sizeof(int)==4, "warning: many of the tests in this file assume a 4-byte integer!z");

using test_signed = test_int;
using test_unsigned = cnl::make_unsigned_t<test_signed>;

using int8 = cnl::set_digits_t<test_signed, 7>;
using uint8 = cnl::set_digits_t<test_unsigned, 8>;
using int16 = cnl::set_digits_t<test_signed, 15>;
using uint16 = cnl::set_digits_t<test_unsigned, 16>;
using int32 = cnl::set_digits_t<test_signed, 31>;
using uint32 = cnl::set_digits_t<test_unsigned, 32>;
using int64 = cnl::set_digits_t<test_signed, 63>;
using uint64 = cnl::set_digits_t<test_unsigned, 64>;

#if defined(CNL_INT128_ENABLED)
using int128 = cnl::set_digits_t<test_signed, 127>;
using uint128 = cnl::set_digits_t<test_unsigned, 128>;
#endif

////////////////////////////////////////////////////////////////////////////////
// imports from cnl

template <typename Rep=test_int, int Exponent=0>
using fixed_point = cnl::fixed_point<Rep, Exponent>;

using cnl::_impl::shift_left;
using cnl::_impl::fp::type::pow2;

template<class Type, cnl::_digits_type MinNumDigits>
using set_digits_t = cnl::set_digits_t<Type, MinNumDigits>;

using cnl::divide;
using cnl::multiply;

using cnl::_impl::identical;

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

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_alternative_specialization)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = fixed_point<uint8>(87.65);
    ASSERT_EQ(static_cast<fixed_point<uint8>>(lhs), fixed_point<uint8>(87.65));
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
// sqrt exception throwing

#if defined(CNL_EXCEPTIONS_ENABLED)

TEST(TOKENPASTE2(TEST_LABEL, sqrt_exception), from_alternative_specialization)
{
    ASSERT_THROW(sqrt(fixed_point<>(-1)), std::invalid_argument);
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

// mixed-mode operations DO lose precision because exponent is more important than significand
static_assert(is_same<std::common_type<float, uint32>::type, float>::value, "incorrect assumption about promotion");

// promotion doesn't always tend towards int
static_assert(is_same<std::common_type<int64, uint32>::type, int64>::value, "incorrect assumption about promotion");
static_assert(is_same<std::common_type<int32, uint64>::type, uint64>::value, "incorrect assumption about promotion");
static_assert(is_same<std::common_type<int8, int8>::type, int8>::value, "incorrect assumption about promotion");
static_assert(is_same<decltype(int8(0)+int8(0)), test_int>::value, "incorrect assumption about promotion");
static_assert(is_same<decltype(int8(0)+int8(0)), test_int>::value, "incorrect assumption about promotion");
static_assert(is_same<decltype(uint8(0)+int8(0)), test_int>::value, "incorrect assumption about promotion");
static_assert(is_same<decltype(uint8(0)+uint8(0)), test_int>::value, "incorrect assumption about promotion");

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::_impl

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::fp::type::shift_left positive RHS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4310)
#endif

static_assert(shift_left<1, int8>(uint8(0))==0, "cnl::shift_left test failed");
static_assert(shift_left<1, int8>(int8(0))==0, "cnl::shift_left test failed");

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(shift_left<8, uint16>((uint16) 0x1234)==0x3400, "cnl::shift_left test failed");
static_assert(shift_left<8, uint16>((uint8) 0x1234)==0x3400, "cnl::shift_left test failed");
static_assert(shift_left<8, uint8>((uint16) 0x1234)==0x0, "cnl::shift_left test failed");
static_assert(shift_left<8, uint16>((uint16) 0x1234)==0x3400, "cnl::shift_left test failed");
static_assert(shift_left<8, uint16>((uint8) 0x1234)==0x3400, "cnl::shift_left test failed");
static_assert(shift_left<8, uint8>((uint16) 0x1234)==0x0, "cnl::shift_left test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(identical(shift_left<8, uint16>((uint16)0x1234), uint16{0xffff}), "cnl::shift_left test failed");
static_assert(shift_left<8, uint16>((uint8)0x1234) == 0xff00, "cnl::shift_left test failed");
static_assert(shift_left<8, uint8>((uint16)0x1234) == 0xff, "cnl::shift_left test failed");
#endif

static_assert(shift_left<8, int16>(-123)==-31488, "cnl::shift_left test failed");

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(shift_left<8, uint16>((uint16)0x1234) == 0xffff, "cnl::shift_left test failed");
static_assert(shift_left<8, uint16>((uint8)0x1234) == 0xff00, "cnl::shift_left test failed");
static_assert(shift_left<8, uint8>((uint16)0x1234) == 0xff, "cnl::shift_left test failed");
#endif

static_assert(shift_left<8, int16>(-123)==-31488, "cnl::shift_left test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::fp::type::shift_left negative RHS

static_assert(shift_left<-8, uint16>((uint16) 0x1234)==0x12, "cnl::shift_left test failed");
static_assert(shift_left<-8, uint8>((uint16) 0x1234)==0x12, "cnl::shift_left test failed");
static_assert(shift_left<-8, int16>(-31488)==-123, "cnl::shift_left test failed");

#if !defined(TEST_THROWING_OVERFLOW)
static_assert(shift_left<-8, uint16>((uint8) 0x1234)==0x0, "cnl::shift_left test failed");
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
static_assert(shift_left<-8, uint16>((uint16) 0x1234)==0x12, "cnl::shift_left test failed");
static_assert(shift_left<-8, uint8>((uint16) 0x1234)==0x12, "cnl::shift_left test failed");
static_assert(shift_left<-8, int16>(-31488)==-123, "cnl::shift_left test failed");

#if !defined(TEST_THROWING_OVERFLOW)
static_assert(shift_left<-8, uint16>((uint8) 0x34)==0x0, "cnl::shift_left test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::fp::type::pow2

static_assert(pow2<float, 0>()==1, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<double, -1>()==.5, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<long double, 1>()==2, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<float, -3>()==.125, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<double, 7>()==128, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<long double, 10>()==1024, "cnl::_impl::fp::type::pow2 test failed");
static_assert(pow2<float, 20>()==1048576, "cnl::_impl::fp::type::pow2 test failed");

////////////////////////////////////////////////////////////////////////////////
// numeric traits

namespace test_is_composite {
    using cnl::is_composite;

    static_assert(is_composite<fixed_point<test_int>>::value, "cnl::is_composite<fixed_point<>> test failed");
    static_assert(cnl::_num_traits_impl::are_composite<fixed_point<test_int>>::value, "cnl::is_composite<fixed_point<>> test failed");
}

namespace test_to_rep {
    using cnl::_impl::to_rep;

    static_assert(identical(to_rep(fixed_point<test_int>{97531}), test_int{97531}), "cnl::to_rep");
    static_assert(identical(to_rep(fixed_point<uint8>{199}), uint8{199}), "cnl::to_rep");
}
namespace test_from_rep {
    using cnl::_impl::from_rep;

    static_assert(identical(from_rep<fixed_point<>>(test_int{0}), fixed_point<>{0}), "from_rep");
    static_assert(identical(from_rep<fixed_point<int16, -10>>(int16{3072}), fixed_point<int16, -10>{test_int{3}}), "from_rep");
    static_assert(!from_rep<fixed_point<test_int, -100>>(test_int{0}), "from_rep");
    static_assert(from_rep<fixed_point<test_int, -100>>(test_int{1}), "from_rep");
    static_assert(!from_rep<fixed_point<test_int, 1000>>(test_int{0}), "from_rep");
    static_assert(from_rep<fixed_point<test_int, 1000>>(test_int{1}), "from_rep");
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// cnl::set_digits_t<fixed_point<>, >

static_assert(identical(set_digits_t<fixed_point<uint8, -8>, 8>{0}, fixed_point<uint8, -8>{0}),
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
static_assert(
        is_same<cnl::set_digits_t<cnl::fixed_point<long unsigned int, -96>, 96u>, cnl::fixed_point<unsigned __int128, -96>>::value,
        "");
#endif

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

#if !defined(TEST_THROWING_OVERFLOW)
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED)
static_assert(fixed_point<int8, -7>(1)!=1.L, "cnl::fixed_point test failed");
#endif
#endif

static_assert(fixed_point<int8, -7>(.5)==.5f, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, -7>(.125f)==.125L, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<int16, -7>(123.125f)==123.125f, "cnl::fixed_point test failed");
#endif
static_assert(fixed_point<int32, -7>(123.125f)==123.125, "cnl::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125l)==123.125f, "cnl::fixed_point test failed");

// exponent == 16
static_assert(fixed_point<uint8, 16>(test_int{ 65536 }) == 65536.f, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint16, 16>(6553.)==0, "cnl::fixed_point test failed");
#endif
static_assert((fixed_point<uint32, 16>(4294967296l))==4294967296.f, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<uint64, 16>(1125895611875328l))==1125895611875328ul, "cnl::fixed_point test failed");
#endif

static_assert(fixed_point<int8, 16>(-65536)==-65536.f, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<int16, 16>(-6553.)==0, "cnl::fixed_point test failed");
#endif
static_assert((fixed_point<int32, 16>(-4294967296l))==-4294967296.f, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<int64, 16>(-1125895611875328l))==-1125895611875328l, "cnl::fixed_point test failed");
#endif

// exponent = 1
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 1>(510)==510, "cnl::fixed_point test failed");
static_assert(fixed_point<uint8, 1>(511)==510, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(123.5)==122, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, 1>(255)==254, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(254)==254, "cnl::fixed_point test failed");
static_assert(fixed_point<int8, 1>(-5)==-4, "cnl::fixed_point test failed");
#endif

// conversion between fixed_point specializations
static_assert(fixed_point<uint8, -4>(fixed_point<int16, -8>(1.5))==1.5, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE)
static_assert(fixed_point<uint16, -8>(fixed_point<int8, -4>(3.25))==3.25, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(fixed_point<uint8, 4>(fixed_point<int16, -4>(768))==768, "cnl::fixed_point test failed");
#endif
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654))>3.1415923f,
        "cnl::fixed_point test failed");
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654))<3.1415927f,
        "cnl::fixed_point test failed");
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(fixed_point<>(-1), "cnl::fixed_point test failed");
static_assert(fixed_point<>(1024), "cnl::fixed_point test failed");
static_assert(!fixed_point<>(0), "cnl::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::_impl::common_type_t

// commonality never occurs when inputs are the same fixed_point type
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<int8, -3>, fixed_point<int8, -3>>, fixed_point<int8, -3>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<int32, -14>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<int64, -48>, fixed_point<int64, -48>>, fixed_point<int64, -48>>::value,
        "cnl::_impl::common_type_t test failed");

// commonality between homogeneous fixed_point types
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<uint8, -4>, fixed_point<int8, -4>>, fixed_point<test_int, -4>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<uint64, -50>, fixed_point<int8, 0>>, fixed_point<uint64, -50>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<int16, -4>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<int16, 0>, fixed_point<uint64, -60>>, fixed_point<uint64, -60>>::value,
        "cnl::_impl::common_type_t test failed");

// commonality between arithmetic and fixed_point types
static_assert(is_same<cnl::_impl::common_type_t<float, fixed_point<int8, -4>>, float>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(is_same<cnl::_impl::common_type_t<double, fixed_point<int32, -14>>, double>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(is_same<cnl::_impl::common_type_t<int8, fixed_point<uint64, -60>>, fixed_point<uint64, -60>>::value,
        "cnl::_impl::common_type_t test failed");

static_assert(
        is_same<cnl::_impl::common_type_t<fixed_point<uint8, -4>, uint32>, fixed_point<test_unsigned, -4>>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(is_same<cnl::_impl::common_type_t<fixed_point<int16, -4>, float>, float>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(is_same<cnl::_impl::common_type_t<fixed_point<int16, 0>, double>, double>::value,
        "cnl::_impl::common_type_t test failed");
static_assert(is_same<
        cnl::_impl::common_type_t<fixed_point<uint8, 10>, test_int>,
        fixed_point<test_int, 0>>::value, "cnl::_impl::common_type_t test failed");

////////////////////////////////////////////////////////////////////////////////
// fixed-point arithmetic support types

namespace test_operate {
    using cnl::_impl::add_op;
    using cnl::_impl::multiply_op;
    using cnl::_impl::divide_op;

    using cnl::_impl::fp::arithmetic::wide_tag;
    using cnl::_impl::fp::arithmetic::lean_tag;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_fixed_point_impl::rep_op_exponent

    using cnl::_impl::fp::arithmetic::rep_op_exponent;

    static_assert(rep_op_exponent<multiply_op, fixed_point<uint8, -4>, fixed_point<uint8, -4>>::value==-8,
            "cnl::_fixed_point_impl::rep_op_exponent test failed");
    static_assert(rep_op_exponent<divide_op, fixed_point<int16, -14>, fixed_point<int8, 0>>::value==-14,
            "cnl::_fixed_point_impl::rep_op_exponent test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_fixed_point_impl::result

    using cnl::_impl::fp::arithmetic::result;

    // cnl::_impl::fp::arithmetic::result::type
    static_assert(identical(
            result<lean_tag, add_op, fixed_point<test_int, 0>, fixed_point<uint8, 10>>::type{12288},
            fixed_point<test_int, 0>{12288}), "cnl::_impl::fp::arithmetic::result test failed");

    static_assert(identical(
            result<wide_tag, multiply_op, fixed_point<uint8, -4>, fixed_point<uint8, -4>>::type{0},
            fixed_point<uint16, -8>{0}), "cnl::_impl::fp::arithmetic::result test failed");
    static_assert(identical(
            result<wide_tag, multiply_op, fixed_point<uint32, 0>, fixed_point<uint32, 0>>::type{1},
            fixed_point<uint64, 0>{1}), "cnl::fixed_point test failed");

    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<int16, -14>, fixed_point<int8, 0>>::type{1.5},
            fixed_point<test_int, -21>{1.5}), "cnl::_impl::fp::arithmetic::result test failed");
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<uint8, -1>, fixed_point<uint8, -3>>::type{15.75},
            fixed_point<test_unsigned, -6>{15.75}), "cnl::fixed_point test failed");
#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
    static_assert(identical(
            result<wide_tag, divide_op, fixed_point<uint32, 0>, fixed_point<uint32, 0>>::type{1},
            fixed_point<uint64, -32>{1}), "cnl::fixed_point test failed");
#endif
    static_assert(identical(
            result<lean_tag, divide_op, fixed_point<int8, -1>, fixed_point<int8, -1>>::type{-15.75},
            fixed_point<test_int, 0>{-15.75}), "cnl::fixed_point test failed");

    // cnl::_impl::fp::arithmetic::result::rep_op_result
    static_assert(identical(
            result<wide_tag, multiply_op, fixed_point<uint8, -1>, fixed_point<uint8, -3>>::rep_op_result{65535},
            test_int{65535}), "cnl::fixed_point test failed");

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::fp::arithmetic::intermediate

    using cnl::_impl::fp::arithmetic::intermediate;

    // cnl::_impl::fp::arithmetic::intermediate::rep_type
    static_assert(identical(
            intermediate<wide_tag, divide_op, fixed_point<uint8, -1>, fixed_point<uint8, -3>>::rep_type{65537},
            test_unsigned{65537}), "cnl::fixed_point test failed");

    // cnl::_impl::fp::arithmetic::intermediate::lhs_type
    static_assert(identical(
            intermediate<wide_tag, multiply_op, fixed_point<uint8, -4>, fixed_point<uint8, -4>>::lhs_type{0},
            fixed_point<uint8, -4>{0}), "cnl::_impl::fp::arithmetic::intermediate test failed");
    static_assert(identical(
            intermediate<wide_tag, multiply_op, fixed_point<uint32, 0>, fixed_point<uint32, 0>>::lhs_type{1},
            fixed_point<uint64, 0>{1}), "cnl::fixed_point test failed");

    static_assert(identical(
            intermediate<wide_tag, divide_op, fixed_point<uint8, -1>, fixed_point<uint8, -3>>::lhs_type{15.75},
            fixed_point<test_unsigned, -9>{15.75}), "cnl::fixed_point test failed");
#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
    static_assert(identical(
            intermediate<wide_tag, divide_op, fixed_point<uint32, 0>, fixed_point<uint32, 0>>::lhs_type{1},
            fixed_point<uint64, -32>{1}), "cnl::fixed_point test failed");
#endif

    static_assert(identical(
            intermediate<lean_tag, add_op, fixed_point<test_int, 0>, fixed_point<uint8, 10>>::lhs_type{12288},
            fixed_point<test_int, 0>{12288}), "cnl::_impl::fp::arithmetic::intermediate test failed");
    static_assert(identical(
            intermediate<lean_tag, add_op, fixed_point<test_int, 0>, fixed_point<uint8, 10>>::rhs_type{12288},
            fixed_point<test_int, 0>{12288}), "cnl::_impl::fp::arithmetic::intermediate test failed");
}

////////////////////////////////////////////////////////////////////////////////
// cnl::multiply

static_assert(std::numeric_limits<uint8>::max()/5==51, "");
static_assert(std::numeric_limits<uint8>::max()/3==85, "");

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(identical(multiply(fixed_point<uint8, -4>{2}, fixed_point<uint8, -4>{7.5}), fixed_point<uint16, -8>{15}),
        "cnl::multiply test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// cnl::divide

static_assert(identical(divide(fixed_point<int16, -14>{1}, fixed_point<int8>{127}), fixed_point<test_int, -14>{1./127}),
        "cnl::divide test failed");

////////////////////////////////////////////////////////////////////////////////
// comparison

// heterogeneous fixed-point to fixed-point comparison
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(-4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<fixed_point<int16, -7>(5.6), "cnl::fixed_point test failed");
#endif
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(!(fixed_point<int8, -3>(-4.5)<fixed_point<int16, -7>(-5.6)), "cnl::fixed_point test failed");
#endif

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, -4>(4.6)>fixed_point<int16, -8>(4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.6)<fixed_point<int16, -8>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(-4.5)), "cnl::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(4.5), "cnl::fixed_point test failed");
static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(-4.5), "cnl::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(4.6)), "cnl::fixed_point test failed");
#endif

// heterogeneous fixed-point to arithmetic comparison
static_assert(fixed_point<uint8, -4>(4.5)==4.5, "cnl::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(!(4==fixed_point<int16, -7>(-4.5)), "cnl::fixed_point test failed");
#endif

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
#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<int32>(123)+fixed_point<uint32>(123))==246, "cnl::fixed_point addition operator test failed");
#endif
static_assert((fixed_point<int32, -16>(123.125)+fixed_point<int32, -16>(123.75))==246.875, "cnl::fixed_point addition operator test failed");

static_assert(identical(fixed_point<int8, -5>{2.125}+fixed_point<int8, -5>{-3.25}, fixed_point<test_int, -5>{-1.125f}),
        "cnl::fixed_point addition operator test failed");

#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(identical(fixed_point<uint8, 10>{10240}+2048, fixed_point<test_int, 0>{12288}), "test failed");
#endif
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(identical(2048+fixed_point<uint8, 10>{10240}, fixed_point<test_int, 0>{12288}),
        "cnl::fixed_point addition operator test failed");
#endif
static_assert(is_same<decltype(2048+fixed_point<uint8, 10>(10240)), fixed_point<test_signed, 0>>::value,
        "cnl::fixed_point addition operator test failed");

static_assert(765.432f+fixed_point<int64, -32>(16777215.996093750)==16777981.428100586, "cnl::fixed_point addition operator test failed");
static_assert(is_same<decltype(765.432f+fixed_point<int64, -32>(16777215.996093750)), double>::value,
        "cnl::fixed_point addition operator test failed");
static_assert(fixed_point<int64, -32>(16777215.996093750)+765.432f==16777981.428100586, "cnl::fixed_point addition operator test failed");
static_assert(is_same<decltype(fixed_point<int64, -32>(16777215.996093750)+765.432f), double>::value,
        "cnl::fixed_point addition operator test failed");

#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(identical(cnl::add(fixed_point<int32, -16>{.5}, 2), fixed_point<int64, -16>{2.5}),
        "cnl::fixed_point addition operator test failed");
static_assert(identical(fixed_point<int32, -16>{.5} + 2, fixed_point<test_int, -16>{2.5}),
        "cnl::fixed_point addition operator test failed");
#endif

// subtraction
static_assert((fixed_point<int32>(999)-fixed_point<int32>(369))==630, "cnl::fixed_point subtraction test failed");
static_assert((fixed_point<int32, -16>(246.875)-fixed_point<int32, -16>(123.75))==123.125, "cnl::fixed_point test failed");
static_assert((fixed_point<int16, -4>(123.125)-fixed_point<int16, -4>(246.875))==-123.75, "cnl::fixed_point test failed");

static_assert(fixed_point<int8, -5>(2.125)-fixed_point<int8, -5>(3.25)==-1.125f, "cnl::fixed_point subtraction test failed");
static_assert(is_same<decltype(fixed_point<int8, -5>(2.125)-fixed_point<int8, -5>(-3.25)), fixed_point<test_int, -5>>::value,
        "cnl::fixed_point subtraction test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 10>(10240)-2048==8192, "cnl::fixed_point subtraction test failed");
#endif
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)-2048), fixed_point<test_signed, 0>>::value,
        "cnl::fixed_point subtraction test failed");
static_assert(765.432f-fixed_point<int64, -32>(16777215.996093750)==-16776450.564086914, "cnl::fixed_point subtraction test failed");
static_assert(is_same<decltype(765.432f-fixed_point<int64, -32>(16777215.996093750)), double>::value,
        "cnl::fixed_point subtraction test failed");

// multiplication
static_assert(identical(multiply(fixed_point<uint8>{0x55}, fixed_point<uint8>{2}), fixed_point<uint16, 0>{0xaa}),
        "cnl::fixed_point test failed");
static_assert(identical(fixed_point<uint8>{0x55} * fixed_point<uint8>{2}, fixed_point<test_int, 0>{0xaa}),
        "cnl::fixed_point test failed");

static_assert(identical(multiply(fixed_point<int32, -16>{123.75}, fixed_point<int32, -16>{44.5}), fixed_point<int64, -32>{5506.875}),
        "cnl::fixed_point test failed");
static_assert(identical(fixed_point<int64, -16>{123.75} * fixed_point<int32, -16>(44.5), fixed_point<int64, -32>{5506.875}),
        "cnl::fixed_point test failed");

static_assert(identical(multiply(fixed_point<int8, -5>{2.125}, fixed_point<int8, -5>{-3.25}), fixed_point<int16, -10>{-6.90625}),
        "cnl::fixed_point multiplication test failed");
static_assert(
        identical(fixed_point<int8, -5>{2.125} * fixed_point<int8, -5>{-3.25}, fixed_point<test_int, -10>{-6.90625}),
        "cnl::fixed_point multiplication test failed");

#if defined(__clang__)
static_assert(identical(multiply(fixed_point<uint8, 10>{10240}, 3u), fixed_point<uint64, 10>{30720}),
        "cnl::fixed_point multiplication test failed");
static_assert(identical(fixed_point<uint8, 10>{10240} * 3u, fixed_point<test_unsigned, 10>{30720}),
        "cnl::fixed_point multiplication test failed");

static_assert(identical(multiply(3u, fixed_point<uint8, 10>{10240}), fixed_point<uint64, 10>{30720}),
        "cnl::fixed_point multiplication test failed");
static_assert(identical(3u * uint8{4}, test_unsigned{12}), "cnl::fixed_point multiplication test failed");
static_assert(identical(3u * fixed_point<uint8, 10>{10240}, fixed_point<test_unsigned, 10>{30720}),
        "cnl::fixed_point multiplication test failed");
#endif

static_assert(-123.654f*fixed_point<int64, -32>(16777215.996093750)==-2074569855.5169766,
        "cnl::fixed_point multiplication test failed");
static_assert(is_same<decltype(765.432f*fixed_point<int64, -32>(16777215.996093750)), double>::value,
        "cnl::fixed_point multiplication test failed");
static_assert(fixed_point<int64, -32>(16777215.996093750)*-123.654f==-2074569855.5169766,
        "cnl::fixed_point multiplication test failed");
static_assert(is_same<decltype(fixed_point<int64, -32>(16777215.996093750)*-123.654f), double>::value,
        "cnl::fixed_point multiplication test failed");

// division
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(identical(fixed_point<int8, -1>{63}/fixed_point<int8, -1>{-4}, fixed_point<test_int, -7>{-15.75}),
        "cnl::fixed_point test failed");
static_assert(identical(fixed_point<test_int, -1>{63}/fixed_point<int8, -1>{-4}, fixed_point<int64, -7>{-15.75}),
        "cnl::fixed_point test failed");
#endif
#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(identical(fixed_point<int8, 1>{-255}/fixed_point<int8, 1>{-8}, fixed_point<test_int, -7>{31.75}),
        "cnl::fixed_point test failed");
static_assert((fixed_point<int8, 1>(-255)/fixed_point<int8, 1>(-8))==31.75, "cnl::fixed_point test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(divide(fixed_point<int32>(-999), fixed_point<int32>(3))==-333, "cnl::fixed_point test failed");
#endif
#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(
        identical(fixed_point<int8, -5>{2.5}/fixed_point<int8, -5>{-4.f}, fixed_point<test_int, -7>{-.625}),
        "cnl::fixed_point division test failed");

static_assert(identical(divide(fixed_point<uint32, 10>{10240}, fixed_point<uint32>{3u}), fixed_point<uint32,
        10>{3413.3333333}), "cnl::fixed_point division test failed");
static_assert(identical(10/fixed_point<uint8, -6>(0.25), fixed_point<int64, -2>{40}),
        "cnl::fixed_point division test failed");
#if ! defined(__GNUG__) // Bug 71504?
static_assert(
        identical(fixed_point<uint8, 10>{10240}/static_cast<uint16>(3), fixed_point<test_unsigned, -6>{3413.3333333}),
        "cnl::fixed_point division test failed");

static_assert(identical(test_int{10}/fixed_point<uint8, -2>{0.25}, fixed_point<int64, -6>{40}),
        "cnl::fixed_point division test failed");
static_assert(identical(divide(10, fixed_point<uint8, -2>{0.25}), fixed_point<test_int, 2>{40}),
        "cnl::fixed_point division test failed");
#endif
#endif

static_assert(16777215.996093750/fixed_point<int64, -32>(-123.654f)==-135678.71712347874,
        "cnl::fixed_point division test failed");
static_assert(is_same<decltype(16777215.996093750/fixed_point<int64, -32>(-123.654f)), double>::value,
        "cnl::fixed_point division test failed");
static_assert(fixed_point<int64, -32>(16777215.996093750)/-123.654f==-135678.71712347874,
        "cnl::fixed_point division test failed");
static_assert(is_same<decltype(fixed_point<int64, -32>(16777215.996093750)/-123.654f), double>::value,
        "cnl::fixed_point division test failed");

#if ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && ! defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(
        identical(fixed_point<uint32, 0>{0xffffffff}/fixed_point<uint32, 0>{0xffffffff}, fixed_point<uint64, -32>{1}),
        "cnl::fixed_point test failed");
static_assert(identical(divide(fixed_point<uint64, 0>{0xFFFFFFFE00000001LL}, fixed_point<uint32, 0>{0xffffffff}),
        fixed_point<uint64, 0>{0xffffffff}), "cnl::fixed_point test failed");
#endif
static_assert(identical(divide(fixed_point<uint32, 0>{0xFFFE0001LL}, fixed_point<uint32, 0>{0xffff}),
        fixed_point<uint32, 0>{0xffff}), "cnl::fixed_point test failed");

namespace test_bitshift {
    // dynamic
    static_assert(identical(fixed_point<int, -4>{2}, fixed_point<uint8_t, -4>{1} << 1), "bitshift test failed");
    static_assert(identical(fixed_point<int, -4>{.5}, fixed_point<uint8_t, -4>{1} >> 1), "bitshift test failed");

    // std::integral_constant
    static_assert(identical(fixed_point<uint8_t, -3>{2}, fixed_point<uint8_t, -4>{1} << std::integral_constant<int, 1>{}), "bitshift test failed");
    static_assert(identical(fixed_point<uint8_t, -5>{.5}, fixed_point<uint8_t, -4>{1} << std::integral_constant<int, -1>{}), "bitshift test failed");

    static_assert(identical(fixed_point<uint8_t, -5>{.5}, fixed_point<uint8_t, -4>{1} >> std::integral_constant<int, 1>{}), "bitshift test failed");
    static_assert(identical(fixed_point<uint8_t, -3>{2}, fixed_point<uint8_t, -4>{1} >> std::integral_constant<int, -1>{}), "bitshift test failed");

    // const_integer
    using namespace cnl::literals;
    static_assert(identical(fixed_point<uint8_t, -3>{2}, fixed_point<uint8_t, -4>{1} << 1_c), "bitshift test failed");
    static_assert(identical(fixed_point<uint8_t, -5>{.5}, fixed_point<uint8_t, -4>{1} << -1_c), "bitshift test failed");

    static_assert(identical(fixed_point<uint8_t, -5>{.5}, fixed_point<uint8_t, -4>{1} >> 1_c), "bitshift test failed");
    static_assert(identical(fixed_point<uint8_t, -3>{2}, fixed_point<uint8_t, -4>{1} >> -1_c), "bitshift test failed");
}

////////////////////////////////////////////////////////////////////////////////
// std::numeric_limits<fixed_point<>>

template<class Rep, int Exponent, class Min, class Max, class Lowest>
constexpr bool test_numeric_limits(Min min, Max max, Lowest lowest)
{
    using fp = fixed_point<Rep, Exponent>;
    using nl = numeric_limits<fp>;
    using rnl = numeric_limits<Rep>;

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

static_assert(numeric_limits<fixed_point<test_int, -256>>::lowest() < -.1e-67, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, -256>>::min() > 0., "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, -256>>::min() < .1e-76, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, -256>>::max() > .1e-67, "std::numeric_limits<fixed_point> test failed");

static_assert(numeric_limits<fixed_point<test_unsigned, -256>>::lowest() == 0., "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, -256>>::min() > 0., "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, -256>>::min() < .1e-76, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, -256>>::max() > .1e-67, "std::numeric_limits<fixed_point> test failed");

static_assert(test_numeric_limits<test_signed, -16>(1/65536.,
        std::numeric_limits<test_signed>::max()/65536.,
        std::numeric_limits<test_signed>::lowest()/65536.), "");

static_assert(test_numeric_limits<test_unsigned, -16>(1/65536.,
        std::numeric_limits<test_unsigned>::max()/65536.,
        std::numeric_limits<test_unsigned>::lowest()/65536.), "");

static_assert(test_numeric_limits<test_signed, 0>(1,
        std::numeric_limits<test_signed>::max(),
        std::numeric_limits<test_signed>::lowest()), "");

static_assert(test_numeric_limits<test_unsigned, 0U>(1U,
        std::numeric_limits<test_unsigned>::max(),
        std::numeric_limits<test_unsigned>::lowest()), "");

static_assert(test_numeric_limits<test_signed, 16>(65536.,
        std::numeric_limits<test_signed>::max()*65536.,
        std::numeric_limits<test_signed>::lowest()*65536.), "");

static_assert(test_numeric_limits<test_unsigned, 16>(65536.,
        std::numeric_limits<test_unsigned>::max()*65536.,
        std::numeric_limits<test_unsigned>::lowest()*65536.), "");

static_assert(numeric_limits<fixed_point<test_int, 256>>::lowest() < -1.e86, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, 256>>::min() > 1.e77, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, 256>>::min() < 1.e78, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_int, 256>>::max() > 1.e86, "std::numeric_limits<fixed_point> test failed");

static_assert(numeric_limits<fixed_point<test_unsigned, 256>>::lowest() == 0., "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, 256>>::min() > 1.e77, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, 256>>::min() < 1.e78, "std::numeric_limits<fixed_point> test failed");
static_assert(numeric_limits<fixed_point<test_unsigned, 256>>::max() > 1.e86, "std::numeric_limits<fixed_point> test failed");

////////////////////////////////////////////////////////////////////////////////
// cnl::sqrt

static_assert(sqrt(fixed_point<uint8>(225))==15, "cnl::sqrt test failed");
static_assert(sqrt(fixed_point<int8>(81))==9, "cnl::sqrt test failed");

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(sqrt(fixed_point<uint8, -1>(4))==2, "cnl::sqrt test failed");
static_assert(sqrt(fixed_point<int8, -2>(9))==3, "cnl::sqrt test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(sqrt(fixed_point<uint8, -4>(4))==2, "cnl::sqrt test failed");
static_assert(static_cast<float>(sqrt(fixed_point<int32, -24>(3.141592654)))>1.7724537849426, "cnl::sqrt test failed");
static_assert(static_cast<float>(sqrt(fixed_point<int32, -24>(3.141592654)))<1.7724537849427, "cnl::sqrt test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// std::leading_bits<fixed_point>

namespace test_used_bits {
    using cnl::used_bits;

    static_assert(used_bits(uint8{10})==4, "cnl::used_bits");
    static_assert(used_bits(fixed_point<uint8, -4>{1}) == 5, "cnl::used_bits");
    static_assert(used_bits(fixed_point<uint8, -3>{2}) == 5, "cnl::used_bits");
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
    using numeric_limits = std::numeric_limits<fixed_point>;
    static constexpr fixed_point min = cnl::_impl::from_rep<fixed_point>(rep(1));
    static_assert(min.data() == rep(1), "all Rep types should be able to store the number 1!");

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
    static_assert(divide(84 * min, 84) == min, "basic arithmetic isn't working");
    static_assert((7 * min) - (4 * min) == divide(6 * min, 2), "basic arithmetic isn't working");
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
};

template struct FixedPointRepTester<int8>;
template struct FixedPointRepTester<uint8>;

template struct FixedPointRepTester<int16>;
template struct FixedPointRepTester<uint16>;

template struct FixedPointRepTester<int32>;
template struct FixedPointRepTester<uint32>;

template struct FixedPointRepTester<int64>;
template struct FixedPointRepTester<uint64>;
