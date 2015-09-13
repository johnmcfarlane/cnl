#include <fixed_point_utils.h>

#include "test_utils.h"

using namespace sg14;

void fixed_point_test()
{
	using namespace std;

	////////////////////////////////////////////////////////////////////////////////
	// copy assignment

	// from fixed_point
	auto rhs = fixed_point<>(123.456);
	auto lhs = rhs;
	ASSERT_EQUAL(lhs, fixed_point<>(123.456));

	// from floating-point type
	lhs = 234.567;
	ASSERT_EQUAL(static_cast<double>(lhs), 234.56698608398438);

	// from integer
	lhs = 543;
	ASSERT_EQUAL(static_cast<int>(lhs), 543);

	// from alternative specialization
	lhs = fixed_point<uint8_t>(87.65);
	ASSERT_EQUAL(static_cast<fixed_point<uint8_t>>(lhs), fixed_point<uint8_t>(87.65));
}

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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// sg14::_impl

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::is_integral

static_assert(_impl::is_integral<int>(), "sg14::_impl::is_integral test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right positive RHS

static_assert(_impl::shift_left<8, std::uint16_t>((std::uint16_t)0x1234) == 0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, std::uint16_t>((std::uint8_t)0x1234) == 0x3400, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, std::uint8_t>((std::uint16_t)0x1234) == 0x0, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<8, std::int16_t>(-123) == -31488, "sg14::_impl::shift_left test failed");

static_assert(_impl::shift_right<8, std::uint16_t>((std::uint16_t)0x1234) == 0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, std::uint16_t>((std::uint8_t)0x1234) == 0x0, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, std::uint8_t>((std::uint16_t)0x1234) == 0x12, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<8, std::int16_t>(-31488) == -123, "sg14::_impl::shift_right test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::_impl::shift_left/right negative RHS

static_assert(_impl::shift_right<-8, std::uint16_t>((std::uint16_t)0x1234) == 0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, std::uint16_t>((std::uint8_t)0x1234) == 0x3400, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, std::uint8_t>((std::uint16_t)0x1234) == 0x0, "sg14::_impl::shift_right test failed");
static_assert(_impl::shift_right<-8, std::int16_t>(-123) == -31488, "sg14::_impl::shift_right test failed");

static_assert(_impl::shift_left<-8, std::uint16_t>((std::uint16_t)0x1234) == 0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, std::uint16_t>((std::uint8_t)0x1234) == 0x0, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, std::uint8_t>((std::uint16_t)0x1234) == 0x12, "sg14::_impl::shift_left test failed");
static_assert(_impl::shift_left<-8, std::int16_t>(-31488) == -123, "sg14::_impl::shift_left test failed");

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
// sg14::_impl::necessary_repr_t

static_assert(std::is_same<_impl::necessary_repr_t<1, false>, std::uint8_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<1, true>, std::int8_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<8, false>, std::uint8_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<8, true>, std::int8_t>::value, "sg14::_impl::necessary_repr_t");

static_assert(std::is_same<_impl::necessary_repr_t<9, false>, std::uint16_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<9, true>, std::int16_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<16, false>, std::uint16_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<16, true>, std::int16_t>::value, "sg14::_impl::necessary_repr_t");

static_assert(std::is_same<_impl::necessary_repr_t<17, false>, std::uint32_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<17, true>, std::int32_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<32, false>, std::uint32_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<32, true>, std::int32_t>::value, "sg14::_impl::necessary_repr_t");

static_assert(std::is_same<_impl::necessary_repr_t<33, false>, std::uint64_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<33, true>, std::int64_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<64, false>, std::uint64_t>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<64, true>, std::int64_t>::value, "sg14::_impl::necessary_repr_t");

#if defined(_SG14_FIXED_POINT_128)
static_assert(std::is_same<_impl::necessary_repr_t<65, false>, unsigned __int128>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<65, true>, __int128>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<128, false>, unsigned __int128>::value, "sg14::_impl::necessary_repr_t");
static_assert(std::is_same<_impl::necessary_repr_t<128, true>, __int128>::value, "sg14::_impl::necessary_repr_t");
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// sg14::fixed_point

////////////////////////////////////////////////////////////////////////////////
// default second template parameter

static_assert(std::is_same<fixed_point<std::int8_t>, fixed_point<std::int8_t, -4>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::uint8_t>, fixed_point<std::uint8_t, -4>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::int16_t>, fixed_point<std::int16_t, -8>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::uint16_t>, fixed_point<std::uint16_t, -8>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::int32_t>, fixed_point<std::int32_t, -16>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::uint32_t>, fixed_point<std::uint32_t, -16>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::int64_t>, fixed_point<std::int64_t, -32>>::value, "sg14::fixed_point test failed");
static_assert(std::is_same<fixed_point<std::uint64_t>, fixed_point<std::uint64_t, -32>>::value, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// default first template parameter

static_assert(std::is_same<fixed_point<int, _impl::num_bits<int>() / -2>, fixed_point<>>::value, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// conversion

// exponent == 0
static_assert(static_cast<float>(make_ufixed<8, 0>(12.34f)) == 12.f, "sg14::fixed_point test failed");
static_assert(static_cast<double>(make_ufixed<16, 0>(12.34f)) == 12.f, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(make_ufixed<32, 0>(12.34f)) == 12.f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(make_ufixed<64, 0>(12.34f)) == 12.f, "sg14::fixed_point test failed");

static_assert(static_cast<double>(make_fixed<7, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(make_fixed<15, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(make_fixed<31, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");
static_assert(static_cast<double>(make_fixed<63, 0>(-12.34f)) == -12.f, "sg14::fixed_point test failed");

// exponent = -1
static_assert(static_cast<float>(fixed_point<std::uint8_t, -1>(127.5)) == 127.5, "sg14::fixed_point test failed");

static_assert(static_cast<float>(fixed_point<std::int8_t, -1>(63.5)) == 63.5, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int8_t, -1>(-63.5)) == -63.5, "sg14::fixed_point test failed");

// exponent == -7
static_assert(static_cast<float>(fixed_point<std::uint8_t, -7>(.125f)) == .125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::uint16_t, -8>(232.125f)) == 232.125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::uint32_t, -7>(232.125f)) == 232.125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::uint64_t, -7>(232.125f)) == 232.125f, "sg14::fixed_point test failed");

static_assert(static_cast<float>(fixed_point<std::int8_t, -7>(.125f)) == .125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int16_t, -7>(123.125f)) == 123.125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int32_t, -7>(123.125f)) == 123.125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int64_t, -7>(123.125f)) == 123.125f, "sg14::fixed_point test failed");

static_assert(static_cast<double>(fixed_point<std::uint8_t, -7>(.125f)) == .125f, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(fixed_point<std::uint16_t, -8>(232.125f)) == 232.125f, "sg14::fixed_point test failed");
static_assert(static_cast<double>(fixed_point<std::uint32_t, -7>(232.125f)) == 232.125f, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(fixed_point<std::uint64_t, -7>(232.125f)) == 232.125f, "sg14::fixed_point test failed");

static_assert(static_cast<long double>(fixed_point<std::int8_t, -7>(1)) != 1.f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int8_t, -7>(.5)) == .5f, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(fixed_point<std::int8_t, -7>(.125f)) == .125f, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::int16_t, -7>(123.125f)) == 123, "sg14::fixed_point test failed");
static_assert(static_cast<long double>(fixed_point<std::int32_t, -7>(123.125f)) == 123.125f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int64_t, -7>(123.125l)) == 123.125f, "sg14::fixed_point test failed");

// exponent == 16
static_assert(static_cast<float>(fixed_point<std::uint8_t, 16>(65536)) == 65536.f, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::uint16_t, 16>(6553.)) == 0, "sg14::fixed_point test failed");
static_assert(static_cast<double>(fixed_point<std::uint32_t, 16>(4294967296l)) == 4294967296.f, "sg14::fixed_point test failed");
static_assert(static_cast<double>(fixed_point<std::uint64_t, 16>(1125895611875328l)) == 1125895611875328l, "sg14::fixed_point test failed");

static_assert(static_cast<float>(fixed_point<std::int8_t, 16>(-65536)) == -65536.f, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::int16_t, 16>(-6553.)) == 0, "sg14::fixed_point test failed");
static_assert(static_cast<double>(fixed_point<std::int32_t, 16>(-4294967296l)) == -4294967296.f, "sg14::fixed_point test failed");
static_assert(static_cast<double>(fixed_point<std::int64_t, 16>(-1125895611875328l)) == -1125895611875328l, "sg14::fixed_point test failed");

// exponent = 1
static_assert(static_cast<int>(fixed_point<std::uint8_t, 1>(510)) == 510, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::uint8_t, 1>(511)) == 510, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::int8_t, 1>(123.5)) == 122, "sg14::fixed_point test failed");

static_assert(static_cast<int>(fixed_point<std::int8_t, 1>(255)) == 254, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::int8_t, 1>(254)) == 254, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::int8_t, 1>(-5)) == -6, "sg14::fixed_point test failed");

// conversion between fixed_point specializations
static_assert(static_cast<float>(make_ufixed<4, 4>(make_fixed<7, 8>(1.5))) == 1.5, "sg14::fixed_point test failed");
static_assert(static_cast<float>(make_ufixed<8, 8>(make_fixed<3, 4>(3.25))) == 3.25, "sg14::fixed_point test failed");
static_assert(static_cast<int>(fixed_point<std::uint8_t, 4>(fixed_point<std::int16_t, -4>(768))) == 768, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::uint64_t, -48>(fixed_point<std::uint32_t, -24>(3.141592654))) > 3.1415923f, "sg14::fixed_point test failed");
static_assert(static_cast<float>(fixed_point<std::uint64_t, -48>(fixed_point<std::uint32_t, -24>(3.141592654))) < 3.1415927f, "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// boolean

static_assert(fixed_point<>(-1), "sg14::fixed_point test failed");
static_assert(fixed_point<>(1024), "sg14::fixed_point test failed");
static_assert(! fixed_point<>(0), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// arithmetic

// addition
static_assert(static_cast<int>((make_fixed<31, 0>(123) + make_fixed<31, 0>(123))) == 246, "sg14::fixed_point test failed");
static_assert(static_cast<float>((make_fixed<15, 16>(123.125) + make_fixed<15, 16>(123.75))) == 246.875, "sg14::fixed_point test failed");

// subtraction
static_assert(static_cast<int>((make_fixed<31, 0>(999) - make_fixed<31, 0>(369))) == 630, "sg14::fixed_point test failed");
static_assert(static_cast<float>((make_fixed<15, 16>(246.875) - make_fixed<15, 16>(123.75))) == 123.125, "sg14::fixed_point test failed");
static_assert(static_cast<float>((make_fixed<11, 4>(123.125) - make_fixed<11, 4>(246.875))) == -123.75, "sg14::fixed_point test failed");

// multiplication
static_assert(static_cast<int>((make_ufixed<8, 0>(0x55) * make_ufixed<8, 0>(2))) == 0xaa, "sg14::fixed_point test failed");
static_assert(static_cast<float>((make_fixed<15, 16>(123.75) * make_fixed<15, 16>(44.5))) == 5506.875, "sg14::fixed_point test failed");
#if defined(_SG14_FIXED_POINT_128)
static_assert(static_cast<int>((make_ufixed<56, 8>(1003006) * make_ufixed<56, 8>(7))) == 7021042, "sg14::fixed_point test failed");
#endif

// division
static_assert(static_cast<float>((fixed_point<std::int8_t, -1>(63) / fixed_point<std::int8_t, -1>(-4))) == -15.5, "sg14::fixed_point test failed");
static_assert(static_cast<int>((fixed_point<std::int8_t, 1>(-255) / fixed_point<std::int8_t, 1>(-8))) == 32, "sg14::fixed_point test failed");
static_assert(static_cast<int>((make_fixed<31, 0>(-999) / make_fixed<31, 0>(3))) == -333, "sg14::fixed_point test failed");
#if defined(_SG14_FIXED_POINT_128)
static_assert(static_cast<int>((fixed_point<std::uint64_t, -8>(65535) / fixed_point<std::uint64_t, -8>(256))) == 255, "sg14::fixed_point test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::fixed_point_promotion_t

static_assert(std::is_same<fixed_point_promotion_t<fixed_point<std::int8_t, -4>>, fixed_point<std::int16_t, -8>>::value, "sg14::fixed_point_promotion_t test failed");
static_assert(std::is_same<fixed_point_promotion_t<fixed_point<std::uint32_t, 44>>, fixed_point<std::uint64_t, 88>>::value, "sg14::fixed_point_promotion_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::fixed_point_demotion_t

static_assert(std::is_same<fixed_point<std::int8_t, -4>, fixed_point_demotion_t<fixed_point<std::int16_t, -9>>>::value, "sg14::fixed_point_demotion_t test failed");
static_assert(std::is_same<fixed_point<std::uint32_t, 44>, fixed_point_demotion_t<fixed_point<std::uint64_t, 88>>>::value, "sg14::fixed_point_demotion_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::make_fixed_from_repr

static_assert(make_fixed_from_repr<std::uint8_t, 8>::integer_digits == 8, "sg14::make_fixed_from_repr test failed");
static_assert(make_fixed_from_repr<std::int32_t, 27>::integer_digits == 27, "sg14::make_fixed_from_repr test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::common_type

// conversion never occurs when inputs are the same type
static_assert(std::is_same<_impl::common_type<fixed_point<std::int8_t, -3>, fixed_point<std::int8_t, -3>>, fixed_point<int8_t, -3>>::value, "sg14::common_type");
static_assert(std::is_same<_impl::common_type<fixed_point<std::int32_t, -14>, fixed_point<std::int32_t, -14>>, fixed_point<int32_t, -14>>::value, "sg14::common_type");
static_assert(std::is_same<_impl::common_type<fixed_point<std::int64_t, -48>, fixed_point<std::int64_t, -48>>, fixed_point<int64_t, -48>>::value, "sg14::common_type");

static_assert(std::is_same<_impl::common_type<fixed_point<std::uint8_t, -4>, fixed_point<std::int8_t, -4>>, fixed_point<int8_t, -3>>::value, "sg14::common_type");
static_assert(std::is_same<_impl::common_type<fixed_point<std::int16_t, -4>, fixed_point<std::int32_t, -14>>, fixed_point<int32_t, -14>>::value, "sg14::common_type");
static_assert(std::is_same<_impl::common_type<fixed_point<std::int16_t, 0>, fixed_point<std::uint64_t, -60>>, fixed_point<int64_t, -48>>::value, "sg14::common_type");

////////////////////////////////////////////////////////////////////////////////
// comparison

static_assert(fixed_point<uint8_t>(4.5) == fixed_point<int16_t>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8_t>(4.5) == fixed_point<int16_t>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8_t>(4.5) != fixed_point<int16_t>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8_t>(4.5) != fixed_point<int16_t>(4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8_t>(4.5) < fixed_point<int16_t>(5.6), "sg14::fixed_point test failed");
static_assert(!(fixed_point<int8_t>(-4.5) < fixed_point<int16_t>(-5.6)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8_t>(4.6) > fixed_point<int16_t>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8_t>(4.6) < fixed_point<int16_t>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8_t>(4.5) <= fixed_point<int16_t>(4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8_t>(4.5) <= fixed_point<int16_t>(-4.5)), "sg14::fixed_point test failed");

static_assert(fixed_point<uint8_t>(4.5) >= fixed_point<int16_t>(4.5), "sg14::fixed_point test failed");
static_assert(fixed_point<uint8_t>(4.5) >= fixed_point<int16_t>(-4.5), "sg14::fixed_point test failed");
static_assert(!(fixed_point<uint8_t>(4.5) >= fixed_point<int16_t>(4.6)), "sg14::fixed_point test failed");

// TODO: Is this acceptable?
static_assert(fixed_point<uint8_t, -1>(.5) == fixed_point<uint8_t, 0>(0), "sg14::fixed_point test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_add_result_t

static_assert(trunc_add_result_t<fixed_point<std::uint8_t, -4>>::integer_digits == 5, "sg14::trunc_add_result_t test failed");
static_assert(trunc_add_result_t<fixed_point<std::int32_t, -25>, 4>::integer_digits == 8, "sg14::trunc_add_result_t test failed");
static_assert(std::is_same<trunc_add_result_t<fixed_point<std::int8_t, 0>, 2>, fixed_point<int8_t, 1>>::value, "sg14::trunc_add_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_add

static_assert(static_cast<int>(trunc_add(make_ufixed<7, 1>(127), make_ufixed<7, 1>(127))) == 254, "sg14::trunc_add test failed");
static_assert(static_cast<float>(trunc_add(make_ufixed<4, 4>(15.5), make_ufixed<4, 4>(14.25), make_ufixed<4, 4>(13.5))) == 43.25, "sg14::trunc_add test failed");
static_assert(static_cast<int>(trunc_add(make_fixed<7, 0>(-128), make_fixed<7, 0>(-128))) == -256, "sg14::trunc_add test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_subtract_result_t

static_assert(std::is_same<trunc_subtract_result_t<make_ufixed<4, 4>>, make_fixed<5, 2>>::value, "sg14::trunc_subtract_result_t test failed");
static_assert(std::is_same<trunc_subtract_result_t<make_fixed<4, 3>, make_ufixed<8, 8>>, make_fixed<9, 6>>::value, "sg14::trunc_subtract_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_subtract

static_assert(static_cast<int>(trunc_subtract(make_ufixed<7, 1>(127.5), make_ufixed<7, 1>(127))) == 0, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_ufixed<7, 1>(127.5), make_ufixed<7, 1>(0))) == 126, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_ufixed<7, 1>(0), make_ufixed<7, 1>(127.5))) == -126, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_fixed<7, 0>(127), make_fixed<7, 0>(127))) == 0, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_fixed<7, 0>(127), make_fixed<7, 0>(-128))) == 254, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_fixed<7, 0>(-128), make_fixed<7, 0>(-128))) == 0, "sg14::trunc_subtract test failed");
static_assert(static_cast<float>(trunc_subtract(make_fixed<7, 0>(-128), make_fixed<7, 0>(127))) == -254, "sg14::trunc_subtract test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_multiply_result_t

static_assert(trunc_multiply_result_t<fixed_point<std::uint8_t, -4>>::integer_digits == 8, "sg14::trunc_multiply_result_t test failed");
static_assert(trunc_multiply_result_t<fixed_point<std::int32_t, -25>>::integer_digits == 12, "sg14::trunc_multiply_result_t test failed");
static_assert(trunc_multiply_result_t<fixed_point<std::uint8_t, 0>>::integer_digits == 16, "sg14::trunc_multiply_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_multiply

static_assert(static_cast<int>(trunc_multiply(make_fixed<7, 8>(127), make_fixed<7, 8>(127))) == 16129, "sg14::trunc_multiply test failed");
static_assert(static_cast<int>(trunc_multiply(make_ufixed<4, 4>(15.9375), make_ufixed<4, 4>(15.9375))) == 254, "sg14::trunc_multiply test failed");
static_assert(static_cast<float>(trunc_multiply(make_ufixed<4, 4>(0.0625), make_ufixed<4, 4>(0.0625))) == 0, "sg14::trunc_multiply test failed");
static_assert(static_cast<float>(trunc_multiply(make_ufixed<8, 0>(1), make_ufixed<8, 0>(1))) == 0, "sg14::trunc_multiply test failed");
static_assert(static_cast<float>(trunc_multiply(make_ufixed<8, 0>(174), make_ufixed<8, 0>(25))) == 4096, "sg14::trunc_multiply test failed");
static_assert(static_cast<int>(trunc_multiply(make_fixed<8, 0, false>(174), make_fixed<6, 2, false>(25))) == 4288, "sg14::trunc_multiply test failed");
static_assert(static_cast<double>(trunc_multiply(make_fixed<4, 3>(15.875), make_fixed<16, 0, false>(65535))) == 1040352, "sg14::trunc_multiply test failed");
static_assert(static_cast<int>(trunc_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-15.875))) == 254, "sg14::trunc_multiply test failed");
static_assert(static_cast<int>(trunc_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-16))) == -256, "sg14::trunc_multiply test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_square_result_t

static_assert(std::is_same<trunc_square_result_t<make_ufixed<4, 4>>, make_ufixed<8, 0>>::value, "sg14::trunc_square_result_t test failed");
static_assert(std::is_same<trunc_square_result_t<make_fixed<6, 25>>, make_ufixed<12, 20>>::value, "sg14::trunc_square_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_square

static_assert(static_cast<int>(trunc_square(make_ufixed<7, 1>(127))) == 16128, "sg14::trunc_square test failed");
static_assert(static_cast<float>(trunc_square(make_ufixed<4, 4>(15.5))) == 240, "sg14::trunc_square test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_sqrt_result_t

static_assert(std::is_same<trunc_sqrt_result_t<make_ufixed<4, 4>>, make_ufixed<2, 6>>::value, "sg14::trunc_sqrt_result_t test failed");
static_assert(sizeof(trunc_sqrt_result_t<make_ufixed<4, 4>>) == sizeof(make_ufixed<2, 6>), "sg14::trunc_sqrt_result_t test failed");

static_assert(std::is_same<trunc_sqrt_result_t<make_ufixed<15, 16>>, make_ufixed<8, 24>>::value, "sg14::trunc_sqrt_result_t test failed");
static_assert(sizeof(trunc_sqrt_result_t<make_ufixed<15, 16>>) == sizeof(make_ufixed<8, 24>), "sg14::trunc_sqrt_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::trunc_sqrt

static_assert(static_cast<int>(trunc_sqrt(make_fixed<14, 1>(16128))) == 126, "sg14::trunc_sqrt test failed");
static_assert(static_cast<float>(trunc_sqrt(make_ufixed<8, 0>(240))) == 15, "sg14::trunc_sqrt test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_multiply_result_t

static_assert(std::is_same<promote_multiply_result_t<make_ufixed<4, 4>>, make_ufixed<8, 8>>::value, "sg14::promote_multiply_result_t test failed");
static_assert(std::is_same<promote_multiply_result_t<make_fixed<6, 25>>, make_fixed<13, 50>>::value, "sg14::promote_multiply_result_t test failed");
static_assert(promote_multiply_result_t<fixed_point<std::uint8_t, 0>>::integer_digits == 16, "sg14::promote_multiply_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_multiply

static_assert(static_cast<int>(promote_multiply(make_fixed<7, 8>(127), make_fixed<7, 8>(127))) == 16129, "sg14::promote_multiply test failed");
static_assert(static_cast<int>(promote_multiply(make_ufixed<4, 4>(15.9375), make_ufixed<4, 4>(15.9375))) == 254, "sg14::promote_multiply test failed");
static_assert(static_cast<float>(promote_multiply(make_ufixed<4, 4>(0.0625), make_ufixed<4, 4>(0.0625))) == 0.00390625, "sg14::promote_multiply test failed");
static_assert(static_cast<float>(promote_multiply(make_ufixed<8, 0>(1), make_ufixed<8, 0>(1))) == 1, "sg14::promote_multiply test failed");
static_assert(static_cast<float>(promote_multiply(make_ufixed<8, 0>(174), make_ufixed<8, 0>(25))) == 4350, "sg14::promote_multiply test failed");
static_assert(static_cast<int>(promote_multiply(make_ufixed<8, 0>(174), make_ufixed<6, 2>(25))) == 4350, "sg14::promote_multiply test failed");
static_assert(static_cast<double>(promote_multiply(make_fixed<4, 3>(15.875), make_ufixed<16, 0>(65535))) == 1040368, "sg14::promote_multiply test failed");
static_assert(static_cast<int>(promote_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-15.875))) == 254, "sg14::promote_multiply test failed");
static_assert(static_cast<int>(promote_multiply(make_fixed<4, 3>(-16), make_fixed<4, 3>(-16))) == 256, "sg14::promote_multiply test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_square_result_t

static_assert(std::is_same<promote_square_result_t<make_ufixed<4, 4>>, make_ufixed<8, 8>>::value, "sg14::promote_square_result_t test failed");
static_assert(std::is_same<promote_square_result_t<make_fixed<6, 25>>, make_ufixed<12, 51>>::value, "sg14::promote_square_result_t test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::promote_square

static_assert(static_cast<float>(promote_square(make_ufixed<7, 1>(127.5))) == 16256.25, "sg14::promote_square test failed");
static_assert(static_cast<float>(promote_square(make_ufixed<4, 4>(15.5))) == 240.25, "sg14::promote_square test failed");
static_assert(static_cast<std::uint64_t>(promote_square(make_fixed<31>(2000000000))) == 4000000000000000000, "sg14::promote_square test failed");

////////////////////////////////////////////////////////////////////////////////
// sg14::sqrt

static_assert(static_cast<int>(sqrt(make_ufixed<8, 0>(225))) == 15, "sg14::sqrt test failed");
static_assert(static_cast<int>(sqrt(make_fixed<7, 0>(81))) == 9, "sg14::sqrt test failed");

static_assert(static_cast<int>(sqrt(make_ufixed<7, 1>(4))) == 2, "sg14::sqrt test failed");
static_assert(static_cast<int>(sqrt(make_fixed<5, 2>(9))) == 3, "sg14::sqrt test failed");

static_assert(static_cast<int>(sqrt(make_ufixed<4, 4>(4))) == 2, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654))) > 1.7724537849426, "sg14::sqrt test failed");
static_assert(static_cast<float>(sqrt(make_fixed<7, 24>(3.141592654))) < 1.7724537849427, "sg14::sqrt test failed");
#if defined(_SG14_FIXED_POINT_128)
static_assert(static_cast<std::int64_t>(sqrt(make_fixed<63, 0>(9223372036854775807))) == 3037000499, "sg14::sqrt test failed");
#endif
