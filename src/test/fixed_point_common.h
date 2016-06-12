
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// This file is an x macro for testing use of fixed_point with a given integer family.
// To use, define test_int to an integer type, e.g. int, and include this file.
// The tests may fail on types which are not compact,
// i.e. where (sizeof(test_int)*CHAR_BIT > sg14::width(test_int)::value).

#include <sg14/bits/int128.h>

#include <gtest/gtest.h>

////////////////////////////////////////////////////////////////////////////////
// imports from std

using std::is_same;
using std::declval;

////////////////////////////////////////////////////////////////////////////////
// integer definitions
//
// depends upon test_signed and test_unsigned defined in including source file

using test_signed = test_int;
using test_unsigned = sg14::make_unsigned<test_signed>::type;

using int8 = sg14::set_width_t<test_signed, 8>;
using uint8 = sg14::set_width_t<test_unsigned, 8>;
using int16 = sg14::set_width_t<test_signed, 16>;
using uint16 = sg14::set_width_t<test_unsigned, 16>;
using int32 = sg14::set_width_t<test_signed, 32>;
using uint32 = sg14::set_width_t<test_unsigned, 32>;
using int64 = sg14::set_width_t<test_signed, 64>;
using uint64 = sg14::set_width_t<test_unsigned, 64>;

#if defined(_GLIBCXX_USE_INT128)
using int128 = sg14::set_width_t<test_signed, 128>;
using uint128 = sg14::set_width_t<test_unsigned, 128>;
#endif

////////////////////////////////////////////////////////////////////////////////
// imports from sg14

namespace _impl {
    // namespace injection avoids noisy change following renaming of internal namespace
    using namespace sg14::_fixed_point_impl;
}

template <typename Rep=test_int, int Exponent=0>
using fixed_point = sg14::fixed_point<Rep, Exponent>;

template<int IntegerDigits, int FractionalDigits = 0, class Archetype = test_signed>
using make_fixed = sg14::make_fixed<IntegerDigits, FractionalDigits, Archetype>;

template<int IntegerDigits, int FractionalDigits = 0, class Archetype = test_unsigned>
using make_ufixed = sg14::make_ufixed<IntegerDigits, FractionalDigits, Archetype>;

template<class Archetype, sg14::_width_type MinNumBits>
using set_width_t = sg14::set_width_t<Archetype, MinNumBits>;

using sg14::divide;
using sg14::multiply;

////////////////////////////////////////////////////////////////////////////////
// copy assignment

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_fixed_point)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = fixed_point<uint32, -16>(123.456);
    ASSERT_EQ(lhs, fixed_point<>(123.456));
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_floating_point)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = 234.567;
    ASSERT_EQ(lhs, 234.56698608398438);
}

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_integer)
{
    auto lhs = fixed_point<uint32, -16>(0);
    lhs = 543;
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
    auto x = make_fixed<7, 8>(22.75);
    ASSERT_EQ(x += 12.5, 35.25L);
    ASSERT_EQ(x -= 35.5, -.25);
    ASSERT_EQ(x /= 4, -.0625);
    ASSERT_EQ(x -= -10, 9.9375f);
    ASSERT_EQ(x *= -3, -29.8125);
    ASSERT_EQ(x -= 0.1875, -30);
}

////////////////////////////////////////////////////////////////////////////////
// sqrt exception throwing

#if defined(SG14_FIXED_POINT_EXCEPTIONS_ENABLED)

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
// sg14::_impl

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::next_size

static_assert(is_same<_impl::next_size<int8>, int16>::value, "sg14::_impl::next_size text failed");
static_assert(is_same<_impl::next_size<uint32>, uint64>::value, "sg14::_impl::next_size text failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::previous_size

static_assert(is_same<_impl::previous_size<int64>, int32>::value, "sg14::_impl::previous_size text failed");
static_assert(is_same<_impl::previous_size<uint16>, uint8>::value, "sg14::_impl::previous_size text failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right positive RHS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4310)
#endif

static_assert(_impl::shift_left<1, int8>(uint8(0))==0, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<1, int8>(int8(0))==0, "sg14::_impl::shift_left test failed");

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(_impl::shift_left<8, uint16>((uint16) 0x1234)==0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint16>((uint8) 0x1234)==0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint8>((uint16) 0x1234)==0x0, "sg14::_impl::shift_left test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(_impl::shift_left<8, uint16>((uint16)0x1234) == 0xffff, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint16>((uint8)0x1234) == 0xff00, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint8>((uint16)0x1234) == 0xff, "sg14::_impl::shift_left test failed");
#endif

static_assert(_impl::shift_left<8, int16>(-123)==-31488, "sg14::_impl::shift_left test failed");

static_assert(_impl::shift_right<8, uint16>((uint16) 0x1234)==0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, uint8>((uint16) 0x1234)==0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, int16>(-31488)==-123, "sg14::_impl::shift_right test failed");

#if !defined(TEST_THROWING_OVERFLOW)
static_assert(_impl::shift_right<8, uint16>((uint8) 0x1234)==0x0, "sg14::_impl::shift_right test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right negative RHS

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(_impl::shift_right<-8, uint16>((uint16) 0x1234)==0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint16>((uint8) 0x1234)==0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint8>((uint16) 0x1234)==0x0, "sg14::_impl::shift_right test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(_impl::shift_right<-8, uint16>((uint16)0x1234) == 0xffff, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint16>((uint8)0x1234) == 0xff00, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint8>((uint16)0x1234) == 0xff, "sg14::_impl::shift_right test failed");
#endif

static_assert(_impl::shift_right<-8, int16>(-123)==-31488, "sg14::_impl::shift_right test failed");

static_assert(_impl::shift_left<-8, uint16>((uint16) 0x1234)==0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, uint8>((uint16) 0x1234)==0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, int16>(-31488)==-123, "sg14::_impl::shift_left test failed");

#if !defined(TEST_THROWING_OVERFLOW)
static_assert(_impl::shift_left<-8, uint16>((uint8) 0x1234)==0x0, "sg14::_impl::shift_left test failed");
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::pow2

static_assert(_impl::pow2<float, 0>()==1, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<double, -1>()==.5, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<long double, 1>()==2, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<float, -3>()==.125, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<double, 7>()==128, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<long double, 10>()==1024, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<float, 20>()==1048576, "sg14::_impl::pow2 test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::capacity

static_assert(_impl::capacity<0>::value==0, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<1>::value==1, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<2>::value==2, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<3>::value==2, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<4>::value==3, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<7>::value==3, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<8>::value==4, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<15>::value==4, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<16>::value==5, "sg14::_impl::capacity test failed");

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// sg14::set_width_t<fixed_point<>, >

static_assert(is_same<set_width_t<fixed_point<uint8, -8>, 8>, fixed_point<uint8, -8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int8, 8>, 16>, fixed_point<int16, 8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint16, -16>, 24>, fixed_point<uint32, -16>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int16, 16>, 32>, fixed_point<int32, 16>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint32, -45>, 40>, fixed_point<uint64, -45>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int32, -8>, 48>, fixed_point<int64, -8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint64, 8>, 56>, fixed_point<uint64, 8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int64, -16>, 64>, fixed_point<int64, -16>>::value,
        "sg14::set_width_t test failed");
#if defined(_GLIBCXX_USE_INT128)
static_assert(is_same<set_width_t<fixed_point<uint8, 16>, 72>, fixed_point<uint128, 16>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int8, -45>, 80>, fixed_point<int128, -45>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint16, -8>, 88>, fixed_point<uint128, -8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int16, 8>, 96>, fixed_point<int128, 8>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint32, -16>, 104>, fixed_point<uint128, -16>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int32, 16>, 112>, fixed_point<int128, 16>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<uint64, -45>, 120>, fixed_point<uint128, -45>>::value,
        "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<fixed_point<int64, -8>, 128>, fixed_point<int128, -8>>::value,
        "sg14::set_width_t test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// sg14::fixed_point

////////////////////////////////////////////////////////////////////////////////
// default second template parameter

static_assert(is_same<fixed_point<int8>, fixed_point<int8, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<uint8>, fixed_point<uint8, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<int16>, fixed_point<int16, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<uint16>, fixed_point<uint16, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<int32>, fixed_point<int32, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<uint32>, fixed_point<uint32, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<int64>, fixed_point<int64, 0>>::value, "sg14::fixed_point test failed");
static_assert(is_same<fixed_point<uint64>, fixed_point<uint64, 0>>::value, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// default first template parameter

static_assert(is_same<fixed_point<test_int, 0>, fixed_point<>>::value, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// conversion

// exponent == 0
static_assert(make_ufixed<8, 0>(12.34f)==12.f, "sg14::fixed_point test failed");
static_assert(make_ufixed<16, 0>(12.34f)==12.f, "sg14::fixed_point test failed");
static_assert(make_ufixed<32, 0>(12.34f)==12.L, "sg14::fixed_point test failed");
static_assert(make_ufixed<64, 0>(12.34f)==12.f, "sg14::fixed_point test failed");

static_assert((make_fixed<7, 0>(-12.34f))==-12.f, "sg14::fixed_point test failed");
static_assert(make_fixed<15, 0>(-12.34f)==-12.L, "sg14::fixed_point test failed");
static_assert(make_fixed<31, 0>(-12.34f)==-12.f, "sg14::fixed_point test failed");
static_assert((make_fixed<63, 0>(-12.34f))==-12.f, "sg14::fixed_point test failed");

// exponent = -1
static_assert(fixed_point<uint8, -1>(127.5)==127.5, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, -1>(63.5)==63.5, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, -1>(-63.5)==-63.5, "sg14::fixed_point test failed");

// exponent == -7
static_assert(fixed_point<uint8, -7>(.125f)==.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f)==232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint32, -7>(232.125f)==232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f)==232.125f, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, -7>(.125f)==.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int16, -7>(123.125f)==123.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int32, -7>(123.125f)==123.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125f)==123.125f, "sg14::fixed_point test failed");

static_assert((fixed_point<uint8, -7>(.125f))==.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f)==232.125L, "sg14::fixed_point test failed");
static_assert((fixed_point<uint32, -7>(232.125f))==232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f)==232.125L, "sg14::fixed_point test failed");

#if !defined(TEST_THROWING_OVERFLOW)
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED)
static_assert(fixed_point<int8, -7>(1)!=1.L, "sg14::fixed_point test failed");
#endif
#endif

static_assert(fixed_point<int8, -7>(.5)==.5f, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, -7>(.125f)==.125L, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<int16, -7>(123.125f)==123.125f, "sg14::fixed_point test failed");
#endif
static_assert(fixed_point<int32, -7>(123.125f)==123.125, "sg14::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125l)==123.125f, "sg14::fixed_point test failed");

// exponent == 16
static_assert(fixed_point<uint8, 16>(65536)==65536.f, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint16, 16>(6553.)==0, "sg14::fixed_point test failed");
#endif
static_assert((fixed_point<uint32, 16>(4294967296l))==4294967296.f, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<uint64, 16>(1125895611875328l))==1125895611875328l, "sg14::fixed_point test failed");
#endif

static_assert(fixed_point<int8, 16>(-65536)==-65536.f, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<int16, 16>(-6553.)==0, "sg14::fixed_point test failed");
#endif
static_assert((fixed_point<int32, 16>(-4294967296l))==-4294967296.f, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<int64, 16>(-1125895611875328l))==-1125895611875328l, "sg14::fixed_point test failed");
#endif

// exponent = 1
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 1>(510)==510, "sg14::fixed_point test failed");
static_assert(fixed_point<uint8, 1>(511)==510, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(123.5)==122, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, 1>(255)==254, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(254)==254, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(-5)==-4, "sg14::fixed_point test failed");
#endif

// conversion between fixed_point specializations
static_assert(make_ufixed<4, 4>(make_fixed<7, 8>(1.5))==1.5, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_NATIVE)
static_assert(make_ufixed<8, 8>(make_fixed<3, 4>(3.25))==3.25, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(fixed_point<uint8, 4>(fixed_point<int16, -4>(768))==768, "sg14::fixed_point test failed");
#endif
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_SATURATED) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS_THROWING)
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654))>3.1415923f,
        "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654))<3.1415927f,
        "sg14::fixed_point test failed");
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(fixed_point<>(-1), "sg14::fixed_point test failed");
static_assert(fixed_point<>(1024), "sg14::fixed_point test failed");
static_assert(!fixed_point<>(0), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::default_arithmtic_policy

static_assert(
        is_same<
                _impl::default_arithmetic_policy::add<
                        fixed_point<uint8, -3>,
                        fixed_point<uint8, -4>>::result_type,
                fixed_point<decltype(std::declval<uint8>()+std::declval<uint8>()), -4>>::value,
        "sg14::_impl::default_arithmtic_policy test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::common_type_t

// commonality never occurs when inputs are the same fixed_point type
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<int8, -3>, fixed_point<int8, -3>>, fixed_point<int8, -3>>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<int32, -14>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<int64, -48>, fixed_point<int64, -48>>, fixed_point<int64, -48>>::value,
        "sg14::_impl::common_type_t test failed");

// commonality between homogeneous fixed_point types
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<uint8, -4>, fixed_point<int8, -4>>, fixed_point<test_int, -4>>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<int16, -4>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<int16, 0>, fixed_point<uint64, -60>>, fixed_point<uint64, -49>>::value,
        "sg14::_impl::common_type_t test failed");

// commonality between arithmetic and fixed_point types
static_assert(is_same<sg14::_impl::common_type_t<float, fixed_point<int8, -4>>, float>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(is_same<sg14::_impl::common_type_t<double, fixed_point<int32, -14>>, double>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(is_same<sg14::_impl::common_type_t<int8, fixed_point<uint64, -60>>, fixed_point<uint64, -57>>::value,
        "sg14::_impl::common_type_t test failed");

static_assert(
        is_same<sg14::_impl::common_type_t<fixed_point<uint8, -4>, uint32>, fixed_point<test_unsigned, 0>>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(is_same<sg14::_impl::common_type_t<fixed_point<int16, -4>, float>, float>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(is_same<sg14::_impl::common_type_t<fixed_point<int16, 0>, double>, double>::value,
        "sg14::_impl::common_type_t test failed");
static_assert(is_same<
        sg14::_impl::common_type_t<fixed_point<uint8, 10>, test_int>,
        fixed_point<test_int, 0>>::value, "sg14::_impl::common_type_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::multiply

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(multiply<make_ufixed<4, 4>>(make_ufixed<4, 4>(2), make_ufixed<4, 4>(7.5))==15,
        "sg14::multiply test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::divide

static_assert(divide<fixed_point<>>(fixed_point<>(15), fixed_point<>(2))==15/2,
        "sg14::multiply test failed");
#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(divide<make_fixed<1, 14>>(make_fixed<1, 14>(1), make_fixed<7, 0>(127))==make_fixed<1, 14>(1./127),
        "sg14::divide test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// comparison

// heterogeneous fixed-point to fixed-point comparison
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)==fixed_point<int16, -7>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)!=fixed_point<int16, -7>(4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<fixed_point<int16, -7>(5.6), "sg14::fixed_point test failed");
#endif
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(!(fixed_point<int8, -3>(-4.5)<fixed_point<int16, -7>(-5.6)), "sg14::fixed_point test failed");
#endif

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, -4>(4.6)>fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.6)<fixed_point<int16, -8>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)<=fixed_point<int16, -8>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5)>=fixed_point<int16, -8>(4.6)), "sg14::fixed_point test failed");
#endif

// TODO: Is this acceptable?
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, -1>(.5)==fixed_point<uint8, 0>(0), "sg14::fixed_point test failed");
#endif

// heterogeneous fixed-point to arithmetic comparison
static_assert(fixed_point<uint8, -4>(4.5)==4.5, "sg14::fixed_point test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(!(4==fixed_point<int16, -7>(-4.5)), "sg14::fixed_point test failed");
#endif

static_assert(4.5f!=fixed_point<int16, -7>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint64, -4>(4.5)!=4.5L), "sg14::fixed_point test failed");

static_assert(4.5<fixed_point<int16, -7>(5.6), "sg14::fixed_point test failed");
static_assert(!(fixed_point<int32, -3>(-4.5)<-5.6), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert((make_fixed<31, 0>(123)+make_fixed<31, 0>(123))==246, "sg14::fixed_point addition operator test failed");
static_assert((make_fixed<15, 16>(123.125)+make_fixed<15, 16>(123.75))==246.875, "sg14::fixed_point addition operator test failed");
#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((make_fixed<24>(123)+make_ufixed<31, 0>(123))==246, "sg14::fixed_point addition operator test failed");
#endif
static_assert((make_fixed<15, 16>(123.125)+make_fixed<15, 16>(123.75))==246.875, "sg14::fixed_point addition operator test failed");

static_assert(make_fixed<2, 5>(2.125)+make_fixed<2, 5>(-3.25)==-1.125f, "sg14::fixed_point addition operator test failed");
static_assert(
        is_same<
                decltype(make_fixed<2, 5, test_int>(2.125)+make_fixed<2, 5, test_int>(-3.25)),
                fixed_point<decltype(std::declval<int8>()+std::declval<int8>()), -5>>::value,
        "sg14::fixed_point addition operator test failed");

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 10>(10240)+2048==12288, "test failed");
#endif
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)+2048), fixed_point<test_signed, 0>>::value,
        "sg14::fixed_point addition operator test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(2048+fixed_point<uint8, 10>(10240)==12288, "sg14::fixed_point addition operator test failed");
#endif
static_assert(is_same<decltype(2048+fixed_point<uint8, 10>(10240)), fixed_point<test_signed, 0>>::value,
        "sg14::fixed_point addition operator test failed");

static_assert(765.432f+make_fixed<31, 32>(16777215.996093750)==16777981.428100586, "sg14::fixed_point addition operator test failed");
static_assert(is_same<decltype(765.432f+make_fixed<31, 32>(16777215.996093750)), double>::value,
        "sg14::fixed_point addition operator test failed");
static_assert(make_fixed<31, 32>(16777215.996093750)+765.432f==16777981.428100586, "sg14::fixed_point addition operator test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750)+765.432f), double>::value,
        "sg14::fixed_point addition operator test failed");

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<int32, -16>(.5)+2==2.5, "sg14::fixed_point addition operator test failed");
#endif

// subtraction
static_assert((make_fixed<31, 0>(999)-make_fixed<31, 0>(369))==630, "sg14::fixed_point subtraction test failed");
static_assert((make_fixed<15, 16>(246.875)-make_fixed<15, 16>(123.75))==123.125, "sg14::fixed_point test failed");
static_assert((make_fixed<11, 4>(123.125)-make_fixed<11, 4>(246.875))==-123.75, "sg14::fixed_point test failed");

static_assert(make_fixed<2, 5>(2.125)-make_fixed<2, 5>(3.25)==-1.125f, "sg14::fixed_point subtraction test failed");
static_assert(is_same<decltype(make_fixed<2, 5, test_int>(2.125)-make_fixed<2, 5, test_int>(-3.25)), fixed_point<test_int, -5>>::value,
        "sg14::fixed_point subtraction test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 10>(10240)-2048==8192, "sg14::fixed_point subtraction test failed");
#endif
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)-2048), fixed_point<test_signed, 0>>::value,
        "sg14::fixed_point subtraction test failed");
static_assert(765.432f-make_fixed<31, 32>(16777215.996093750)==-16776450.564086914, "sg14::fixed_point subtraction test failed");
static_assert(is_same<decltype(765.432f-make_fixed<31, 32>(16777215.996093750)), double>::value,
        "sg14::fixed_point subtraction test failed");

// multiplication
static_assert((make_ufixed<8, 0>(0x55)*make_ufixed<8, 0>(2))==0xaa, "sg14::fixed_point test failed");
static_assert((make_fixed<15, 16>(123.75)*make_fixed<15, 16>(44.5))==5506.875, "sg14::fixed_point test failed");

static_assert(make_fixed<2, 5>(2.125)*make_fixed<2, 5>(-1.75f)==-3.71875, "sg14::fixed_point multiplication test failed");
static_assert(is_same<decltype(make_fixed<2, 5, test_int>(2.125)*make_fixed<2, 5, test_int>(-3.25)), fixed_point<test_int, -5>>::value,
        "sg14::fixed_point multiplication test failed");

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 10>(10240)*3u==30720, "sg14::fixed_point multiplication test failed");
#endif
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)*3u), fixed_point<test_unsigned, 10>>::value,
        "sg14::fixed_point multiplication test failed");
#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(3u*fixed_point<uint8, 10>(10240)==30720, "sg14::fixed_point multiplication test failed");
#endif
static_assert(is_same<decltype(3u*fixed_point<uint8, 10>(10240)), fixed_point<test_unsigned, 10>>::value,
        "sg14::fixed_point multiplication test failed");

static_assert(-123.654f*make_fixed<31, 32>(16777215.996093750)==-2074569855.5169766,
        "sg14::fixed_point multiplication test failed");
static_assert(is_same<decltype(765.432f*make_fixed<31, 32>(16777215.996093750)), double>::value,
        "sg14::fixed_point multiplication test failed");
static_assert(make_fixed<31, 32>(16777215.996093750)*-123.654f==-2074569855.5169766,
        "sg14::fixed_point multiplication test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750)*-123.654f), double>::value,
        "sg14::fixed_point multiplication test failed");

// division
static_assert(fixed_point<int8, -1>(63) / fixed_point<int8, -1>(-4) == -15.5, "sg14::fixed_point test failed");

static_assert(
        is_same<decltype(declval<fixed_point<int8, 1>>()
                /declval<fixed_point<int8, 1>>()), fixed_point<test_int, 1>>::value,
        "std::fixed_point test failed");
#if defined(TEST_NATIVE_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<int8, 1>(-255)/fixed_point<int8, 1>(-8))==30, "sg14::fixed_point test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((fixed_point<int8, 1>(-255) / fixed_point<int8, 1>(-8)) == 30, "sg14::fixed_point test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert((make_fixed<31, 0>(-999)/make_fixed<31, 0>(3))==-333, "sg14::fixed_point test failed");
static_assert(make_fixed<2, 5>(2.5)/make_fixed<2, 5>(-4.f)==-.625, "sg14::fixed_point division test failed");
#endif
static_assert(is_same<decltype(make_fixed<2, 5, test_int>(2.5)/make_fixed<2, 5, test_int>(-4.f)), fixed_point<test_int, -5>>::value,
        "sg14::fixed_point division test failed");

#if !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(fixed_point<uint8, 10>(10240)/3u==3072, "sg14::fixed_point division test failed");
#endif
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240)/3u), fixed_point<test_unsigned, 10>>::value,
        "sg14::fixed_point division test failed");
TEST(TOKENPASTE2(TEST_LABEL, division), int_uint8) {
    int n(10);
    fixed_point<uint8, -2> d(0.25);
    ASSERT_EQ(n / d, 40.L);
}
static_assert(is_same<decltype(10/fixed_point<uint8, -2>(0.25)), fixed_point<test_signed, -2>>::value,
        "sg14::fixed_point division test failed");

static_assert(16777215.996093750/make_fixed<31, 32>(-123.654f)==-135678.71712347874,
        "sg14::fixed_point division test failed");
static_assert(is_same<decltype(16777215.996093750/make_fixed<31, 32>(-123.654f)), double>::value,
        "sg14::fixed_point division test failed");
static_assert(make_fixed<31, 32>(16777215.996093750)/-123.654f==-135678.71712347874,
        "sg14::fixed_point division test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750)/-123.654f), double>::value,
        "sg14::fixed_point division test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::sqrt

static_assert(sqrt(make_ufixed<8, 0>(225))==15, "sg14::sqrt test failed");
static_assert(sqrt(make_fixed<7, 0>(81))==9, "sg14::sqrt test failed");

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(sqrt(make_ufixed<7, 1>(4))==2, "sg14::sqrt test failed");
static_assert(sqrt(make_fixed<5, 2>(9))==3, "sg14::sqrt test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW) && !defined(TEST_IGNORE_MSVC_INTERNAL_ERRORS)
static_assert(sqrt(make_ufixed<4, 4>(4))==2, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654)))>1.7724537849426, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654)))<1.7724537849427, "sg14::sqrt test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/cDZIcB1LNfE/heaucUIAAgAJ

static constexpr make_fixed<15, 16> x{1.5};
static constexpr auto y = 6.5*x - 4;
static_assert(y == 5.75, "usage test failed");

////////////////////////////////////////////////////////////////////////////////
// FixedPointTester

template <class Rep, int Exponent>
struct FixedPointTester {
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

#if ! defined(_MSC_VER)
    // simply assignment to and from underlying representation
    using numeric_limits = std::numeric_limits<fixed_point>;
    static constexpr fixed_point min = fixed_point::from_data(rep(1));
    static_assert(min.data() == rep(1), "all Rep types should be able to store the number 1!");

    // sg14::_impl::widen_integer_result_t
    static_assert(
            sizeof(_impl::widen_integer_result_t<fixed_point>) == 2 * sizeof(fixed_point),
            "widen_integer_result_t did not result in type that was twice the size");
    static_assert(
            _impl::widen_integer_result_t<fixed_point>::integer_digits > fixed_point::integer_digits,
            "widen_integer_result_t did not result in type with more integer digits");
    static_assert(
            _impl::widen_integer_result_t<fixed_point>::fractional_digits == fixed_point::fractional_digits,
            "widen_integer_result_t did not result in type with same number of fractional digits");

    // sg14::_impl::widen_fractional_result_t
    static_assert(
            sizeof(_impl::widen_fractional_result_t<fixed_point>) == 2 * sizeof(fixed_point),
            "widen_fractional_result_t did not result in type that was twice the size");
    static_assert(
            _impl::widen_fractional_result_t<fixed_point>::integer_digits == fixed_point::integer_digits,
            "widen_fractional_result_t did not result in type with same number of integer digits");
    static_assert(
            _impl::widen_fractional_result_t<fixed_point>::fractional_digits > fixed_point::fractional_digits,
            "widen_fractional_result_t did not result in type with more fractional digits");

    // unary common_type_t
    static_assert(is_same<
                    sg14::_impl::common_type_t<fixed_point>,
                    ::fixed_point<
                            typename sg14::common_type<Rep>::type,
                            Exponent>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    static_assert(
            is_same<
                    fixed_point,
                    sg14::_impl::common_type_t<fixed_point>>::value,
            "... and that rule should be to do nothing very exciting at all");

    // binary common_type_t
    static_assert(
            is_same<
                    fixed_point,
                    sg14::_impl::common_type_t<fixed_point, fixed_point>>::value,
            "a fixed point specialization follows the same implicit promotion rules as its Rep");

    // for convenience, fixed_point API assumes binary and unary homogeneous common_types are the same
    static_assert(
            is_same<
                    sg14::_impl::common_type_t<fixed_point>,
                    sg14::_impl::common_type_t<fixed_point, fixed_point>>::value,
            "bad assumption about binary sg14::_impl::common_type_t");

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

    // assorted tests of +, -, * and /
    static_assert(min + min == 2 * min, "basic arithmetic isn't working");
    static_assert((84 * min) / 84 == min, "basic arithmetic isn't working");
    static_assert((7 * min) - (4 * min) == (6 * min) / 2, "basic arithmetic isn't working");
#endif
};

template <typename Rep>
struct FixedPointRepTester {
    FixedPointTester<Rep, -100> _0;
    FixedPointTester<Rep, -10> _1;
    FixedPointTester<Rep, -1> _2;
    FixedPointTester<Rep, 0> _3;
    FixedPointTester<Rep, 1> _4;
    FixedPointTester<Rep, 10> _5;
    FixedPointTester<Rep, 100> _6;
};

template struct FixedPointRepTester<int8>;
template struct FixedPointRepTester<uint8>;

template struct FixedPointRepTester<int16>;
template struct FixedPointRepTester<uint16>;

template struct FixedPointRepTester<int32>;
template struct FixedPointRepTester<uint32>;

#if defined(_GLIBCXX_USE_INT128)
template struct FixedPointRepTester<int64>;
template struct FixedPointRepTester<uint64>;
#endif
