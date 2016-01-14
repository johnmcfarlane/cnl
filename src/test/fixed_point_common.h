
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include <fixed_point.h>

////////////////////////////////////////////////////////////////////////////////
// imports from std

using std::is_same;
using std::common_type;

////////////////////////////////////////////////////////////////////////////////
// imports from sg14

using namespace sg14;





////////////////////////////////////////////////////////////////////////////////
// copy assignment

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

TEST(TOKENPASTE2(TEST_LABEL, copy_assignment), from_fixed_point)
{
	auto lhs = fixed_point<uint32, -16>(0);
	lhs = fixed_point<uint32, -16>(123.456);
	EXPECT_EQ(lhs, fixed_point<>(123.456));
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

#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
 TEST(TOKENPASTE2(TEST_LABEL, sqrt_exception), from_alternative_specialization)
{
	ASSERT_THROW(sqrt(fixed_point<>(-1)), std::invalid_argument);
}
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// language assumptions

// default rounding style is truncation
static_assert(static_cast<int>(3.9) == 3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(3.0) == 3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(2.9) == 2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(2.0) == 2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(1.9) == 1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(1.0) == 1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(0.9) == 0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(0.0) == 0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-0.0) == 0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-0.9) == 0, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-1.0) == -1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-1.9) == -1, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-2.0) == -2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-2.9) == -2, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-3.0) == -3, "incorrect assumption about default rounding");
static_assert(static_cast<int>(-3.9) == -3, "incorrect assumption about default rounding");

// mixed-mode operations DO lose precision because exponent is more important than significand
static_assert(is_same<common_type<float, uint32>::type, float>::value, "incorrect assumption about promotion");

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// sg14::_impl

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::next_size

static_assert(is_same<_impl::next_size<int8_t>, int16_t>::value, "sg14::_impl::next_size text failed");
static_assert(is_same<_impl::next_size<uint32_t>, uint64_t>::value, "sg14::_impl::next_size text failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::previous_size

static_assert(is_same<_impl::previous_size<int64_t>, int32_t>::value, "sg14::_impl::previous_size text failed");
static_assert(is_same<_impl::previous_size<uint16_t>, uint8_t>::value, "sg14::_impl::previous_size text failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right positive RHS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4310)
#endif

static_assert(_impl::shift_left<1, int8>(uint8(0)) == 0, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<1, int8>(int8(0)) == 0, "sg14::_impl::shift_left test failed");

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(_impl::shift_left<8, uint16>((uint16)0x1234) == 0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint16>((uint8)0x1234) == 0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint8>((uint16)0x1234) == 0x0, "sg14::_impl::shift_left test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(_impl::shift_left<8, uint16>((uint16)0x1234) == 0xffff, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint16>((uint8)0x1234) == 0xff00, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, uint8>((uint16)0x1234) == 0xff, "sg14::_impl::shift_left test failed");
#endif

static_assert(_impl::shift_left<8, int16>(-123) == -31488, "sg14::_impl::shift_left test failed");

static_assert(_impl::shift_right<8, uint16>((uint16)0x1234) == 0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, uint8>((uint16)0x1234) == 0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, int16>(-31488) == -123, "sg14::_impl::shift_right test failed");

#if ! defined(TEST_THROWING_OVERFLOW)
static_assert(_impl::shift_right<8, uint16>((uint8)0x1234) == 0x0, "sg14::_impl::shift_right test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right negative RHS

#if defined(TEST_NATIVE_OVERFLOW)
static_assert(_impl::shift_right<-8, uint16>((uint16)0x1234) == 0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint16>((uint8)0x1234) == 0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint8>((uint16)0x1234) == 0x0, "sg14::_impl::shift_right test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert(_impl::shift_right<-8, uint16>((uint16)0x1234) == 0xffff, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint16>((uint8)0x1234) == 0xff00, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, uint8>((uint16)0x1234) == 0xff, "sg14::_impl::shift_right test failed");
#endif

static_assert(_impl::shift_right<-8, int16>(-123) == -31488, "sg14::_impl::shift_right test failed");

static_assert(_impl::shift_left<-8, uint16>((uint16)0x1234) == 0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, uint8>((uint16)0x1234) == 0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, int16>(-31488) == -123, "sg14::_impl::shift_left test failed");

#if ! defined(TEST_THROWING_OVERFLOW)
static_assert(_impl::shift_left<-8, uint16>((uint8)0x1234) == 0x0, "sg14::_impl::shift_left test failed");
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::pow2

static_assert(_impl::pow2<float, 0>() == 1, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<double, -1>() == .5, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<long double, 1>() == 2, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<float, -3>() == .125, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<double, 7>() == 128, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<long double, 10>() == 1024, "sg14::_impl::pow2 test failed");
static_assert(_impl::pow2<float, 20>() == 1048576, "sg14::_impl::pow2 test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::capacity

static_assert(_impl::capacity<0>::value == 0, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<1>::value == 1, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<2>::value == 2, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<3>::value == 2, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<4>::value == 3, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<7>::value == 3, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<8>::value == 4, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<15>::value == 4, "sg14::_impl::capacity test failed");
static_assert(_impl::capacity<16>::value == 5, "sg14::_impl::capacity test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::sufficient_repr

static_assert(is_same<_impl::sufficient_repr<1, test_unsigned>, uint8>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<1, test_signed>, int8>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<8, test_unsigned>, uint8>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<8, test_signed>, int8>::value, "sg14::_impl::sufficient_repr");

static_assert(is_same<_impl::sufficient_repr<9, test_unsigned>, uint16>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<9, test_signed>, int16>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<16, test_unsigned>, uint16>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<16, test_signed>, int16>::value, "sg14::_impl::sufficient_repr");

static_assert(is_same<_impl::sufficient_repr<17, test_unsigned>, uint32>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<17, test_signed>, int32>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<32, test_unsigned>, uint32>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<32, test_signed>, int32>::value, "sg14::_impl::sufficient_repr");

static_assert(is_same<_impl::sufficient_repr<33, test_unsigned>, uint64>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<33, test_signed>, int64>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<64, test_unsigned>, uint64>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<64, test_signed>, int64>::value, "sg14::_impl::sufficient_repr");

#if defined(_GLIBCXX_USE_INT128)
static_assert(is_same<_impl::sufficient_repr<65, test_unsigned>, uint128>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<65, test_signed>, int128>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<128, test_unsigned>, uint128>::value, "sg14::_impl::sufficient_repr");
static_assert(is_same<_impl::sufficient_repr<128, test_signed>, int128>::value, "sg14::_impl::sufficient_repr");
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

static_assert(is_same<fixed_point<int, 0>, fixed_point<>>::value, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// conversion

// exponent == 0
static_assert(make_ufixed<8, 0>(12.34f) == 12.f, "sg14::fixed_point test failed");
static_assert(make_ufixed<16, 0>(12.34f) == 12.f, "sg14::fixed_point test failed");
static_assert(make_ufixed<32, 0>(12.34f) == 12.L, "sg14::fixed_point test failed");
static_assert(make_ufixed<64, 0>(12.34f) == 12.f, "sg14::fixed_point test failed");

static_assert((make_fixed<7, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");
static_assert(make_fixed<15, 0>(-12.34f) == -12.L, "sg14::fixed_point test failed");
static_assert(make_fixed<31, 0>(-12.34f) == -12.f, "sg14::fixed_point test failed");
static_assert((make_fixed<63, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");

// exponent = -1
static_assert(fixed_point<uint8, -1>(127.5) == 127.5, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, -1>(63.5) == 63.5, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, -1>(-63.5) == -63.5, "sg14::fixed_point test failed");

// exponent == -7
static_assert(fixed_point<uint8, -7>(.125f) == .125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f) == 232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint32, -7>(232.125f) == 232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f) == 232.125f, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, -7>(.125f) == .125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int16, -7>(123.125f) == 123.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int32, -7>(123.125f) == 123.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125f) == 123.125f, "sg14::fixed_point test failed");

static_assert((fixed_point<uint8, -7>(.125f)) == .125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint16, -8>(232.125f) == 232.125L, "sg14::fixed_point test failed");
static_assert((fixed_point<uint32, -7>(232.125f)) == 232.125f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -7>(232.125f) == 232.125L, "sg14::fixed_point test failed");

#if ! defined(TEST_THROWING_OVERFLOW)
static_assert(fixed_point<int8, -7>(1) != 1.L, "sg14::fixed_point test failed");
#endif

static_assert(fixed_point<int8, -7>(.5) == .5f, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, -7>(.125f) == .125L, "sg14::fixed_point test failed");
static_assert(fixed_point<int16, -7>(123.125f) != 123, "sg14::fixed_point test failed");
static_assert(fixed_point<int32, -7>(123.125f) == 123.125, "sg14::fixed_point test failed");
static_assert(fixed_point<int64, -7>(123.125l) == 123.125f, "sg14::fixed_point test failed");

// exponent == 16
static_assert(fixed_point<uint8, 16>(65536) == 65536.f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint16, 16>(6553.) == 0, "sg14::fixed_point test failed");
static_assert((fixed_point<uint32, 16>(4294967296l)) == 4294967296.f, "sg14::fixed_point test failed");
static_assert((fixed_point<uint64, 16>(1125895611875328l)) == 1125895611875328l, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, 16>(-65536) == -65536.f, "sg14::fixed_point test failed");
static_assert(fixed_point<int16, 16>(-6553.) == 0, "sg14::fixed_point test failed");
static_assert((fixed_point<int32, 16>(-4294967296l)) == -4294967296.f, "sg14::fixed_point test failed");
static_assert((fixed_point<int64, 16>(-1125895611875328l)) == -1125895611875328l, "sg14::fixed_point test failed");

// exponent = 1
static_assert(fixed_point<uint8, 1>(510) == 510, "sg14::fixed_point test failed");
static_assert(fixed_point<uint8, 1>(511) == 510, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(123.5) == 122, "sg14::fixed_point test failed");

static_assert(fixed_point<int8, 1>(255) == 254, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(254) == 254, "sg14::fixed_point test failed");
static_assert(fixed_point<int8, 1>(-5) == -6, "sg14::fixed_point test failed");

// conversion between fixed_point specializations
static_assert(make_ufixed<4, 4>(make_fixed<7, 8>(1.5)) == 1.5, "sg14::fixed_point test failed");
static_assert(make_ufixed<8, 8>(make_fixed<3, 4>(3.25)) == 3.25, "sg14::fixed_point test failed");
static_assert(fixed_point<uint8, 4>(fixed_point<int16, -4>(768)) == 768, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654)) > 3.1415923f, "sg14::fixed_point test failed");
static_assert(fixed_point<uint64, -48>(fixed_point<uint32, -24>(3.141592654)) < 3.1415927f, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(fixed_point<>(-1), "sg14::fixed_point test failed");
static_assert(fixed_point<>(1024), "sg14::fixed_point test failed");
static_assert(! fixed_point<>(0), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert((make_fixed<31, 0>(123) + make_fixed<31, 0>(123)) == 246, "sg14::fixed_point test failed");
static_assert((make_fixed<15, 16>(123.125) + make_fixed<15, 16>(123.75)) == 246.875, "sg14::fixed_point test failed");
static_assert((make_fixed<24>(123) + make_ufixed<31, 0>(123)) == 246, "sg14::fixed_point test failed");
static_assert((make_fixed<15, 16>(123.125) + make_fixed<15, 16>(123.75)) == 246.875, "sg14::fixed_point test failed");

// subtraction
static_assert((make_fixed<31, 0>(999) - make_fixed<31, 0>(369)) == 630, "sg14::fixed_point test failed");
static_assert((make_fixed<15, 16>(246.875) - make_fixed<15, 16>(123.75)) == 123.125, "sg14::fixed_point test failed");
static_assert((make_fixed<11, 4>(123.125) - make_fixed<11, 4>(246.875)) == -123.75, "sg14::fixed_point test failed");

// multiplication
static_assert((make_ufixed<8, 0>(0x55) * make_ufixed<8, 0>(2)) == 0xaa, "sg14::fixed_point test failed");
static_assert((make_fixed<15, 16>(123.75) * make_fixed<15, 16>(44.5)) == 5506.875, "sg14::fixed_point test failed");

// division
constexpr auto divide_result = _impl::shift_left<1, int8>(int8(0));

static_assert((fixed_point<int8, -1>(63) / fixed_point<int8, -1>(-4)) == -16, "sg14::fixed_point test failed");

#if defined(TEST_NATIVE_OVERFLOW)
static_assert((fixed_point<int8, 1>(-255) / fixed_point<int8, 1>(-8)) == 32, "sg14::fixed_point test failed");
#endif

#if defined(TEST_SATURATED_OVERFLOW)
static_assert((fixed_point<int8, 1>(-255) / fixed_point<int8, 1>(-8)) == 30, "sg14::fixed_point test failed");
#endif

static_assert((make_fixed<31, 0>(-999) / make_fixed<31, 0>(3)) == -333, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_result

static_assert(is_same<promote_result<fixed_point<int8, -4>>, fixed_point<int16, -8>>::value, "sg14::promote_result test failed");
static_assert(is_same<promote_result<fixed_point<uint32, 44>>, fixed_point<uint64, 88>>::value, "sg14::promote_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::demote_result

static_assert(is_same<fixed_point<int8, -4>, demote_result<fixed_point<int16, -9>>>::value, "sg14::demote_result test failed");
static_assert(is_same<fixed_point<uint32, 44>, demote_result<fixed_point<uint64, 88>>>::value, "sg14::demote_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::make_fixed_from_repr

static_assert(make_fixed_from_repr<uint8, 8>::integer_digits == 8, "sg14::make_fixed_from_repr test failed");
static_assert(make_fixed_from_repr<int32, 27>::integer_digits == 27, "sg14::make_fixed_from_repr test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::common_repr_type

static_assert(is_same<_impl::common_repr_type<uint8, uint8>, uint8>::value, "sg14::_impl::common_repr_type test failed");
static_assert(is_same<_impl::common_repr_type<int8, uint64>, int64>::value, "sg14::_impl::common_repr_type test failed");
static_assert(is_same<_impl::common_repr_type<uint32, int8>, int32>::value, "sg14::_impl::common_repr_type test failed");
static_assert(is_same<_impl::common_repr_type<int16, int16>, int16>::value, "sg14::_impl::common_repr_type test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::common_type

// commonality never occurs when inputs are the same fixed_point type
static_assert(is_same<_impl::common_type<fixed_point<int8, -3>, fixed_point<int8, -3>>, fixed_point<int8, -3>>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<int32, -14>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<int64, -48>, fixed_point<int64, -48>>, fixed_point<int64, -48>>::value, "sg14::_impl::common_type test failed");

// commonality between homogeneous fixed_point types
static_assert(is_same<_impl::common_type<fixed_point<uint8, -4>, fixed_point<int8, -4>>, fixed_point<int8, -3>>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<int16, -4>, fixed_point<int32, -14>>, fixed_point<int32, -14>>::value, "v");
static_assert(is_same<_impl::common_type<fixed_point<int16, 0>, fixed_point<uint64, -60>>, fixed_point<int64, -48>>::value, "sg14::_impl::common_type test failed");

// commonality between arithmetic and fixed_point types
static_assert(is_same<_impl::common_type<float, fixed_point<int8, -4>>, float>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<double, fixed_point<int32, -14>>, double>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<int8, fixed_point<uint64, -60>>, fixed_point<uint64, -60>>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<uint8, -4>, uint32>, fixed_point<uint8, -4>>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<int16, -4>, float>, float>::value, "sg14::_impl::common_type test failed");
static_assert(is_same<_impl::common_type<fixed_point<int16, 0>, double>, double>::value, "sg14::_impl::common_type test failed");

// commonality between two non-fixed-point types (won't compile)
//static_assert(is_same<_impl::common_type<float, float>, float>::value, "sg14::_impl::common_type test failed");
//static_assert(is_same<_impl::common_type<double, uint16>, double>::value, "sg14::_impl::common_type test failed");
//static_assert(is_same<_impl::common_type<int8, int8>, fixed_point<int64, -56>>::value, "sg14::_impl::common_type test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::multiply

static_assert(_impl::multiply<make_ufixed<4, 4>>(make_ufixed<4, 4>(2), make_ufixed<4, 4>(7.5)) == 15, "sg14::_impl::multiply test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::divide

static_assert(_impl::divide<fixed_point<>>(fixed_point<>(15), fixed_point<>(2)) == 15 / 2, "sg14::_impl::multiply test failed");
static_assert(_impl::divide<make_fixed<1, 14>>(make_fixed<1, 6>(1), make_fixed<7, 0>(127)) == make_fixed<1, 14>(1. / 127), "sg14::trunc_divide test failed");

////////////////////////////////////////////////////////////////////////////////
// comparison

// heterogeneous fixed-point to fixed-point comparison
static_assert(fixed_point<uint8, -4>(4.5) == fixed_point<int16, -7>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5) == fixed_point<int16, -7>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5) != fixed_point<int16, -7>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5) != fixed_point<int16, -7>(4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5) < fixed_point<int16, -7>(5.6), "sg14::fixed_point test failed");
static_assert(!(fixed_point<int8, -3>(-4.5) < fixed_point<int16, -7>(-5.6)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.6) > fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.6) < fixed_point<int16, -8>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5) <= fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5) <= fixed_point<int16, -8>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8, -4>(4.5) >= fixed_point<int16, -8>(4.5), "sg14::fixed_point test failed");
static_assert(fixed_point<uint8, -4>(4.5) >= fixed_point<int16, -8>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8, -4>(4.5) >= fixed_point<int16, -8>(4.6)), "sg14::fixed_point test failed");

// TODO: Is this acceptable?
static_assert(fixed_point<uint8, -1>(.5) == fixed_point<uint8, 0>(0), "sg14::fixed_point test failed");

// heterogeneous fixed-point to arithmetic comparison
static_assert(fixed_point<uint8, -4>(4.5) == 4.5, "sg14::fixed_point test failed");
static_assert(!(4 == fixed_point<int16, -7>(-4.5)), "sg14::fixed_point test failed");

static_assert(4.5f != fixed_point<int16, -7>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint64, -4>(4.5) != 4.5L), "sg14::fixed_point test failed");

static_assert(4.5 < fixed_point<int16, -7>(5.6), "sg14::fixed_point test failed");
static_assert(!(fixed_point<int32, -3>(-4.5) < -5.6), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// arithmetic operators

// addition
static_assert(make_fixed<2, 5>(2.125) + make_fixed<2, 5>(-3.25) == -1.125f, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<2, 5>(2.125) + make_fixed<2, 5>(-3.25)), make_fixed<2, 5>>::value, "arithmetic operators test failed");

static_assert(fixed_point<uint8, 10>(10240) + 2048 == 12288, "arithmetic operators test failed");
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240) + 2048), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");
static_assert(2048 + fixed_point<uint8, 10>(10240) == 12288, "arithmetic operators test failed");
static_assert(is_same<decltype(2048 + fixed_point<uint8, 10>(10240)), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");

static_assert(765.432f + make_fixed<31, 32>(16777215.996093750) == 16777981.428100586, "arithmetic operators test failed");
static_assert(is_same<decltype(765.432f + make_fixed<31, 32>(16777215.996093750)), double>::value, "arithmetic operators test failed");
static_assert(make_fixed<31, 32>(16777215.996093750) + 765.432f == 16777981.428100586, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750) + 765.432f), double>::value, "arithmetic operators test failed");

static_assert(fixed_point<int32,-16>(.5) + 2 == 2.5, "arithmetic operators test failed");

// subtraction
static_assert(make_fixed<2, 5>(2.125) - make_fixed<2, 5>(3.25) == -1.125f, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<2, 5>(2.125) - make_fixed<2, 5>(-3.25)), make_fixed<2, 5>>::value, "arithmetic operators test failed");
static_assert(fixed_point<uint8, 10>(10240) - 2048 == 8192, "arithmetic operators test failed");
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240) - 2048), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");
static_assert(765.432f - make_fixed<31, 32>(16777215.996093750) == -16776450.564086914, "arithmetic operators test failed");
static_assert(is_same<decltype(765.432f - make_fixed<31, 32>(16777215.996093750)), double>::value, "arithmetic operators test failed");

// multiplication
static_assert(make_fixed<2, 5>(2.125) * make_fixed<2, 5>(-1.75f) == -3.71875, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<2, 5>(2.125) * make_fixed<2, 5>(-3.25)), make_fixed<2, 5>>::value, "arithmetic operators test failed");

static_assert(fixed_point<uint8, 10>(10240) * 3u == 30720, "arithmetic operators test failed");
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240) * 3u), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");
static_assert(3u * fixed_point<uint8, 10>(10240) == 30720, "arithmetic operators test failed");
static_assert(is_same<decltype(3u * fixed_point<uint8, 10>(10240)), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");

static_assert(-123.654f * make_fixed<31, 32>(16777215.996093750) == -2074569855.5169766, "arithmetic operators test failed");
static_assert(is_same<decltype(765.432f * make_fixed<31, 32>(16777215.996093750)), double>::value, "arithmetic operators test failed");
static_assert(make_fixed<31, 32>(16777215.996093750) * -123.654f == -2074569855.5169766, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750) * -123.654f), double>::value, "arithmetic operators test failed");

// division
static_assert(make_fixed<2, 5>(2.5) / make_fixed<2, 5>(-4.f) == -.625, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<2, 5>(2.5) / make_fixed<2, 5>(-4.f)), make_fixed<2, 5>>::value, "arithmetic operators test failed");

static_assert(fixed_point<uint8, 10>(10240) / 3u == 3072, "arithmetic operators test failed");
static_assert(is_same<decltype(fixed_point<uint8, 10>(10240) / 3u), fixed_point<uint8, 10>>::value, "arithmetic operators test failed");
static_assert(10 / fixed_point<uint8, -2>(0.25) == 40.L, "arithmetic operators test failed");
static_assert(is_same<decltype(10 / fixed_point<uint8, -2>(0.25)), fixed_point<uint8, -2>>::value, "arithmetic operators test failed");

static_assert(16777215.996093750 / make_fixed<31, 32>(-123.654f) == -135678.71712347874, "arithmetic operators test failed");
static_assert(is_same<decltype(16777215.996093750 / make_fixed<31, 32>(-123.654f)), double>::value, "arithmetic operators test failed");
static_assert(make_fixed<31, 32>(16777215.996093750) / -123.654f == -135678.71712347874, "arithmetic operators test failed");
static_assert(is_same<decltype(make_fixed<31, 32>(16777215.996093750) / -123.654f), double>::value, "arithmetic operators test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_add_result

static_assert(trunc_add_result<fixed_point<uint8, -4>>::integer_digits == 5, "sg14::trunc_add_result test failed");
static_assert(trunc_add_result<fixed_point<int32, -25>, 4>::integer_digits == 8, "sg14::trunc_add_result test failed");
static_assert(is_same<trunc_add_result<fixed_point<int8, 0>, 2>, fixed_point<int8, 1>>::value, "sg14::trunc_add_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_add

static_assert(trunc_add(make_ufixed<7, 1>(127), make_ufixed<7, 1>(127)) == 254, "sg14::trunc_add test failed");
static_assert(trunc_add(make_ufixed<4, 4>(15.5), make_ufixed<4, 4>(14.25), make_ufixed<4, 4>(13.5)) == 43.25, "sg14::trunc_add test failed");
static_assert(trunc_add(make_fixed<7, 0>(-128), make_fixed<7, 0>(-128)) == -256, "sg14::trunc_add test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_subtract_result

static_assert(is_same<trunc_subtract_result<make_ufixed<4, 4>>, make_fixed<5, 2>>::value, "sg14::trunc_subtract_result test failed");
static_assert(is_same<trunc_subtract_result<make_fixed<4, 3>, make_ufixed<8, 8>>, make_fixed<9, 6>>::value, "sg14::trunc_subtract_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_subtract

static_assert(trunc_subtract(make_ufixed<7, 1>(127.5), make_ufixed<7, 1>(127)) == 0, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_ufixed<7, 1>(127.5), make_ufixed<7, 1>(0)) == 126.f, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_ufixed<7, 1>(0), make_ufixed<7, 1>(127.5)) == -126.f, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_fixed<7, 0>(127), make_fixed<7, 0>(127)) == 0.f, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_fixed<7, 0>(127), make_fixed<7, 0>(-128)) == 254.f, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_fixed<7, 0>(-128), make_fixed<7, 0>(-128)) == 0.f, "sg14::trunc_subtract test failed");
static_assert(trunc_subtract(make_fixed<7, 0>(-128), make_fixed<7, 0>(127)) == -254.f, "sg14::trunc_subtract test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_multiply_result

static_assert(trunc_multiply_result<fixed_point<uint8, -4>>::integer_digits == 8, "sg14::trunc_multiply_result test failed");
static_assert(trunc_multiply_result<fixed_point<int32, -25>>::integer_digits == 12, "sg14::trunc_multiply_result test failed");
static_assert(trunc_multiply_result<fixed_point<uint8, 0>>::integer_digits == 16, "sg14::trunc_multiply_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_multiply

static_assert(trunc_multiply(make_fixed<7, 8>(127), make_fixed<7, 8>(127)) == 16129, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_ufixed<4, 4>(15.9375), make_ufixed<4, 4>(15.9375)) == 254, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_ufixed<4, 4>(0.0625), make_ufixed<4, 4>(0.0625)) == 0.f, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_ufixed<8, 0>(1), make_ufixed<8, 0>(1)) == 0.f, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_ufixed<8, 0>(174), make_ufixed<8, 0>(25)) == 4096.f, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_ufixed<8, 0>(174), make_ufixed<6, 2>(25)) == 4288, "sg14::trunc_multiply test failed");
static_assert((trunc_multiply(make_fixed<4, 3>(15.875), make_ufixed<16, 0>(65535))) == 1040352, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-15.875)) == 254, "sg14::trunc_multiply test failed");
static_assert(trunc_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-16)) == -256, "sg14::trunc_multiply test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_divide_result

static_assert(trunc_divide_result<fixed_point<uint8, -4>>::integer_digits == 8, "sg14::trunc_divide_result test failed");
static_assert(trunc_divide_result<make_fixed<4, 3>>::integer_digits == 7, "sg14::trunc_divide_result test failed");
static_assert(trunc_divide_result<fixed_point<int32, -25>>::integer_digits == 31, "sg14::trunc_divide_result test failed");
static_assert(trunc_divide_result<fixed_point<uint8, 0>>::integer_digits == 8, "sg14::trunc_divide_result test failed");
static_assert(is_same<trunc_divide_result<make_fixed<15, 0, int8>, make_fixed<7, 8, int8>>, fixed_point<int16, 8>>::value, "sg14::trunc_divide_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_divide

static_assert(trunc_divide(make_fixed<15, 0>(16129), make_fixed<15, 0>(127)) == 127, "sg14::trunc_divide test failed");
static_assert(trunc_divide(make_ufixed<8, 0>(240), make_ufixed<4, 4>(.9375)) == 256, "sg14::trunc_divide test failed");
static_assert(trunc_divide(make_ufixed<4, 4>(0.0625), make_ufixed<4, 4>(0.0625)) == 1.f, "sg14::trunc_divide test failed");
static_assert(trunc_divide(make_ufixed<8, 0>(0), make_ufixed<8, 0>(1)) == 0.f, "sg14::trunc_divide test failed");
static_assert(trunc_divide(fixed_point<uint16, 0>(4096), make_ufixed<8, 0>(25)) == 163., "sg14::trunc_divide test failed");
static_assert(trunc_divide(make_ufixed<14, 0>(4288), make_ufixed<6, 2>(25)) == 171, "sg14::trunc_divide test failed");
static_assert((trunc_divide(make_fixed<20>(1040352), make_ufixed<16, 0>(65535))) == 15.87451171875, "sg14::trunc_divide test failed");
static_assert(trunc_divide(make_fixed<15>(254), make_fixed<4, 3>(-15.875)) == -16, "sg14::trunc_divide test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_reciprocal_result

static_assert(is_same<trunc_reciprocal_result<fixed_point<uint8, -4>>, fixed_point<uint8, -3>>::value, "sg14::trunc_reciprocal_result test failed");
static_assert(is_same<trunc_reciprocal_result<make_fixed<4, 3>>, make_fixed<4, 3>>::value, "sg14::trunc_reciprocal_result test failed");
static_assert(is_same<trunc_reciprocal_result<fixed_point<int32, -25>>, fixed_point<int32, -5>>::value, "sg14::trunc_reciprocal_result test failed");
static_assert(is_same<trunc_reciprocal_result<fixed_point<uint8, 0>>, fixed_point<uint8, -7>>::value, "sg14::trunc_reciprocal_result test failed");
static_assert(is_same<trunc_reciprocal_result<make_fixed<15, 0, int8>>, fixed_point<int16, -14>>::value, "sg14::trunc_reciprocal_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_reciprocal

static_assert(trunc_reciprocal(make_ufixed<4, 4>(4)) == .25, "sg14::trunc_reciprocal test failed");
static_assert(trunc_reciprocal(make_fixed<7, 0>(127)) == 0, "sg14::trunc_reciprocal test failed");
static_assert(trunc_reciprocal(make_fixed<4, 3>(.125)) == 8, "sg14::trunc_reciprocal test failed");
static_assert(trunc_reciprocal(make_ufixed<4, 4>(0.0625)) == 16, "sg14::trunc_reciprocal test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_square_result

static_assert(is_same<trunc_square_result<make_ufixed<4, 4>>, make_ufixed<8, 0>>::value, "sg14::trunc_square_result test failed");
static_assert(is_same<trunc_square_result<make_fixed<6, 25>>, make_ufixed<12, 20>>::value, "sg14::trunc_square_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_square

static_assert(trunc_square(make_ufixed<7, 1>(127)) == 16128, "sg14::trunc_square test failed");
static_assert(trunc_square(make_ufixed<4, 4>(15.5)) == 240.f, "sg14::trunc_square test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_sqrt_result

static_assert(is_same<trunc_sqrt_result<make_ufixed<4, 4>>, make_ufixed<2, 6>>::value, "sg14::trunc_sqrt_result test failed");
static_assert(sizeof(trunc_sqrt_result<make_ufixed<4, 4>>) == sizeof(make_ufixed<2, 6>), "sg14::trunc_sqrt_result test failed");

static_assert(is_same<trunc_sqrt_result<make_ufixed<15, 16>>, make_ufixed<8, 24>>::value, "sg14::trunc_sqrt_result test failed");
static_assert(sizeof(trunc_sqrt_result<make_ufixed<15, 16>>) == sizeof(make_ufixed<8, 24>), "sg14::trunc_sqrt_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_sqrt

static_assert(trunc_sqrt(make_fixed<14, 1>(16128)) == 126.5, "sg14::trunc_sqrt test failed");
static_assert(trunc_sqrt(make_ufixed<8, 0>(240)) == 15.f, "sg14::trunc_sqrt test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_shift_left / trunc_shift_right

static_assert(trunc_shift_left<8>(fixed_point<uint16>(0x1234)) == 0x123400, "sg14::trunc_sqrt test failed");
static_assert(trunc_shift_left<-2>(fixed_point<uint16>(128)) == 32, "sg14::trunc_sqrt test failed");
static_assert(trunc_shift_right<-8>(fixed_point<uint16>(0x1234)) == 0x123400, "sg14::trunc_sqrt test failed");
static_assert(trunc_shift_right<2>(fixed_point<uint16>(128)) == 32, "sg14::trunc_sqrt test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_add_result

static_assert(promote_add_result<fixed_point<uint8, -4>>::integer_digits == 5, "sg14::promote_add_result test failed");
static_assert(promote_add_result<fixed_point<int32, -25>, 4>::integer_digits == 8, "sg14::promote_add_result test failed");
static_assert(is_same<promote_add_result<fixed_point<int8, 0>, 2>, fixed_point<int16, -7>>::value, "sg14::promote_add_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_add

static_assert(promote_add(make_ufixed<7, 1>(127), make_ufixed<7, 1>(127)) == 254, "sg14::promote_add test failed");
static_assert(promote_add(make_ufixed<4, 4>(15.5), make_ufixed<4, 4>(14.25), make_ufixed<4, 4>(13.5)) == 43.25, "sg14::promote_add test failed");
static_assert(promote_add(make_fixed<7, 0>(-128), make_fixed<7, 0>(-128)) == -256, "sg14::promote_add test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_subtract_result

static_assert(is_same<promote_subtract_result<make_ufixed<4, 4>>, make_fixed<5, 10>>::value, "sg14::promote_subtract_result test failed");
static_assert(is_same<promote_subtract_result<make_fixed<4, 3>>, make_fixed<5, 10>>::value, "sg14::promote_subtract_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_subtract

static_assert(promote_subtract(make_ufixed<7, 1>(127), make_ufixed<7, 1>(127)) == 0, "sg14::promote_subtract test failed");
static_assert(promote_subtract(make_ufixed<4, 4>(15.5), make_ufixed<4, 4>(14.25)) == 1.25, "sg14::promote_subtract test failed");
static_assert(promote_subtract(make_fixed<7, 0>(-128), make_fixed<7, 0>(127)) == -255, "sg14::promote_subtract test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_multiply_result

static_assert(is_same<promote_multiply_result<make_ufixed<4, 4>>, make_ufixed<8, 8>>::value, "sg14::promote_multiply_result test failed");
static_assert(is_same<promote_multiply_result<make_fixed<6, 25>>, make_fixed<13, 50>>::value, "sg14::promote_multiply_result test failed");
static_assert(promote_multiply_result<fixed_point<uint8, 0>>::integer_digits == 16, "sg14::promote_multiply_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_multiply

static_assert(promote_multiply(make_fixed<7, 8>(127), make_fixed<7, 8>(127)) == 16129, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_ufixed<4, 4>(15.9375), make_ufixed<4, 4>(15.9375)) == 254.00390625, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_ufixed<4, 4>(0.0625), make_ufixed<4, 4>(0.0625)) == 0.00390625f, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_ufixed<8, 0>(1), make_ufixed<8, 0>(1)) == 1.f, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_ufixed<8, 0>(174), make_ufixed<8, 0>(25)) == 4350.f, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_ufixed<8, 0>(174), make_ufixed<6, 2>(25)) == 4350, "sg14::promote_multiply test failed");
static_assert((promote_multiply(make_fixed<4, 3>(15.875), make_ufixed<16, 0>(65535))) == 1040368, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-15.875)) == 254, "sg14::promote_multiply test failed");
static_assert(promote_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-16)) == 256, "sg14::promote_multiply test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_divide

static_assert(promote_divide(make_fixed<2, 5>(2.5), make_fixed<2, 5>(-4.f)) == -.625, "arithmetic operators test failed");
static_assert(is_same<decltype(promote_divide(make_fixed<2, 5>(2.5), make_fixed<2, 5>(-4.f))), make_fixed<5, 10>>::value, "arithmetic operators test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_square_result

static_assert(is_same<promote_square_result<make_ufixed<4, 4>>, make_ufixed<8, 8>>::value, "sg14::promote_square_result test failed");
static_assert(is_same<promote_square_result<make_fixed<6, 25>>, make_ufixed<12, 51>>::value, "sg14::promote_square_result test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_square

static_assert(promote_square(make_ufixed<7, 1>(127.5)) == 16256.25f, "sg14::promote_square test failed");
static_assert(promote_square(make_ufixed<4, 4>(15.5)) == 240.25f, "sg14::promote_square test failed");
static_assert(static_cast<uint64>(promote_square(make_fixed<31, 0, int8>(2000000000))) == 4000000000000000000ULL, "sg14::promote_square test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::sqrt

static_assert(sqrt(make_ufixed<8, 0>(225)) == 15, "sg14::sqrt test failed");
static_assert(sqrt(make_fixed<7, 0>(81)) == 9, "sg14::sqrt test failed");

static_assert(sqrt(make_ufixed<7, 1>(4)) == 2, "sg14::sqrt test failed");
static_assert(sqrt(make_fixed<5, 2>(9)) == 3, "sg14::sqrt test failed");

static_assert(sqrt(make_ufixed<4, 4>(4)) == 2, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654))) > 1.7724537849426, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654))) < 1.7724537849427, "sg14::sqrt test failed");
