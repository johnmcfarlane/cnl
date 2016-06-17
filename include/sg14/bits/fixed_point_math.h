//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief some cmath specializations for `sg14::fixed_point` type;

#ifndef FIXED_POINT_MATH_H_
#define FIXED_POINT_MATH_H_

#include <sg14/fixed_point.h>
#include <type_traits>

namespace sg14 {

namespace _fixed_point_impl {

template<typename fp>
constexpr auto rounding_conversion(double d) {
	make_fixed<fp::integer_digits, fp::fractional_digits + 1> in{d};
	return fp::from_data((in.data() + 1) >> 1);
}

//If the exponent is not negative, there is no fractional part,
//so this is always zero
template<typename Rep, int Exponent,
			typename std::enable_if<(Exponent >= 0), int>::type dummy = 0>
inline auto exp2m1_0to1(fixed_point<Rep, Exponent>) {
	return fixed_point<Rep, Exponent>{0};
}

//Computes 2^x - 1 for a number x between 0 and 1, strictly less than 1
template<typename Rep, int Exponent
 ,	typename std::enable_if<(Exponent < 0), int>::type dummy = 0
>
inline auto exp2m1_0to1(fixed_point<Rep, Exponent> x) {

	//Build the type with the same number of bits, all fractional,
	//and unsigned. That should be enough to exactly hold enough bits
	//to guarantee bit-accurate results
	using urep = typename std::make_unsigned<Rep>::type;
	constexpr int bits = std::numeric_limits<urep>::digits;
	using im = fixed_point<urep, -bits>; //The intermediate value type

	//TODO: template magic to get the coefficients automatically
	//from the number of bits of precision
	//Define the coefficients as constexpr,
	//to make sure they're converted to fp
	//at compile time
	constexpr im a1{rounding_conversion<im>(0.6931471860838825)};
	constexpr im a2{rounding_conversion<im>(0.2402263846181129)};
	constexpr im a3{rounding_conversion<im>(0.055505126858894846)};
	constexpr im a4{rounding_conversion<im>(0.009614017013719252)};
	constexpr im a5{rounding_conversion<im>(0.0013422634797558564)};
	constexpr im a6{rounding_conversion<im>(0.00014352314226313836)};
	constexpr im a7{rounding_conversion<im>(0.000021498763160402416)};

	im xf{x}; //Important: convert the type once, to keep every multiply from costing a cast

	//Use a polynomial min-max approximation to generate the exponential of
	//the fractional part. Note that the constant 1 of the polynomial is added later,
	//this gives us one more bit of precision here for free
	return xf * (a1 + xf * (a2 + xf * (a3 + xf * (a4 + xf * (a5 + xf *(a6 + xf * a7)) ) )));

}

template<typename Rep, int Exponent>
constexpr auto floor(fixed_point<Rep, Exponent> x) {
	return Rep{(x.data() ) >> -Exponent};
}

}// namespace _fixed_point_impl

template<typename Rep, int Exponent>
constexpr auto exp2(fixed_point<Rep, Exponent> x) {
	using fp = fixed_point<Rep, Exponent>; // The input type

	//Round _down_, not toward zero
	const auto intpart = _fixed_point_impl::floor(x);

	//Early exit: if the shift would be at least the width
	//of the type, no need to calculate the fractional part,
	//it will anyway be entirely shifted out.
	//Further, shifting by more than the width of the type
	//is undefined behaviour
	if (intpart <= Exponent)
		return fp::from_data(1);

	//The hard part: exponentiate the fraction
	const auto fracpart = (x - intpart);
	const auto fracexp = _fixed_point_impl::exp2m1_0to1<Rep, Exponent>(fracpart); //Compute the power of the fractional part

	//Calculate the final result by shifting the fractional part around.
	//Remember to add the -1 which is left out to get 1 bit more resolution
	return fp::from_data((fracexp.data()
						   >> (-fracexp.exponent + Exponent - intpart))
			          + (Rep{1} << (intpart - Exponent))); //The constant term must be one, to make integer powers correct

}

}



#endif /* FIXED_POINT_MATH_H_ */
