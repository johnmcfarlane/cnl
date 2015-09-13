#if ! defined(_SG14_FIXED_POINT_UTILS_H)
#define _SG14_FIXED_POINT_UTILS_H 1

#include "fixed_point.h"

#include <cmath>
#include <istream>
#include <ostream>

namespace sg14
{
	////////////////////////////////////////////////////////////////////////////////
	// sg14::is_fixed_point

	template <class T>
	struct is_fixed_point;

	template <class T>
	struct is_fixed_point
		: public std::integral_constant<bool, false> {};

	template <class ReprType, int Exponent>
	struct is_fixed_point <fixed_point<ReprType, Exponent>>
		: public std::integral_constant<bool, true> {};

	////////////////////////////////////////////////////////////////////////////////
	// sg14::abs

	template <class ReprType, int Exponent, typename std::enable_if<_impl::is_signed<ReprType>::value, int>::type Dummy = 0>
	constexpr fixed_point<ReprType, Exponent>
	abs(const fixed_point<ReprType, Exponent> & x) noexcept
	{
		return (x.data() >= 0) ? x : - x;
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::trig
	//
	// Placeholder implementations fall back on <cmath> functions which is slow
	// due to conversion to and from floating-point types; also inconvenient as
	// many <cmath> functions are not constexpr.

	namespace _impl
	{
		template <class ReprType, int Exponent, _impl::get_float_t<sizeof(ReprType)>(*F)(_impl::get_float_t<sizeof(ReprType)>)>
		constexpr fixed_point<ReprType, Exponent>
		crib(const fixed_point<ReprType, Exponent> & x) noexcept
		{
			using floating_point = _impl::get_float_t<sizeof(ReprType)>;
			return static_cast<fixed_point<ReprType, Exponent>>(F(static_cast<floating_point>(x)));
		}
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent>
	sin(const fixed_point<ReprType, Exponent> & x) noexcept
	{
		return _impl::crib<ReprType, Exponent, std::sin>(x);
	}

	template <class ReprType, int Exponent>
	constexpr fixed_point<ReprType, Exponent>
	cos(const fixed_point<ReprType, Exponent> & x) noexcept
	{
		return _impl::crib<ReprType, Exponent, std::cos>(x);
	}

	////////////////////////////////////////////////////////////////////////////////
	// sg14::fixed_point streaming - (placeholder implementation)

	template <class ReprType, int Exponent>
	::std::ostream & operator << (::std::ostream & out, const fixed_point<ReprType, Exponent> & fp)
	{
		return out << static_cast<long double>(fp);
	}

	template <class ReprType, int Exponent>
	::std::istream & operator >> (::std::istream & in, fixed_point<ReprType, Exponent> & fp)
	{
		long double ld;
		in >> ld;
		fp = ld;
		return in;
	}
}

#endif	// defined(_SG14_FIXED_POINT_UTILS_H)
