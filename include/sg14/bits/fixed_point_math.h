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
constexpr fp rounding_conversion(double d) {
	using one_longer = make_fixed<fp::integer_digits, fp::fractional_digits + 1>;
	return fp::from_data((one_longer{d}.data() + 1) >> 1);
}

template<typename fp>
using unsigned_rep = typename std::make_unsigned<typename fp::rep>::type;

template<typename fp>
using make_largest_ufraction = fixed_point<unsigned_rep<fp>, -std::numeric_limits<unsigned_rep<fp>>::digits>;

static_assert(std::is_same<make_largest_ufraction<fixed_point<int32_t, -15>>, fixed_point<uint32_t, -32>>::value, "");

//TODO: template magic to get the coefficients automatically
//from the number of bits of precision
//Define the coefficients as constexpr,
//to make sure they're converted to fp
//at compile time
template<typename fp>
struct poly_coeffs {
	static constexpr fp a1{rounding_conversion<fp>(0.6931471860838825)};
	static constexpr fp a2{rounding_conversion<fp>(0.2402263846181129)};
	static constexpr fp a3{rounding_conversion<fp>(0.055505126858894846)};
	static constexpr fp a4{rounding_conversion<fp>(0.009614017013719252)};
	static constexpr fp a5{rounding_conversion<fp>(0.0013422634797558564)};
	static constexpr fp a6{rounding_conversion<fp>(0.00014352314226313836)};
	static constexpr fp a7{rounding_conversion<fp>(0.000021498763160402416)};
};

template<typename fp>
constexpr fp poly_coeffs<fp>::a1;
template<typename fp>
constexpr fp poly_coeffs<fp>::a2;
template<typename fp>
constexpr fp poly_coeffs<fp>::a3;
template<typename fp>
constexpr fp poly_coeffs<fp>::a4;
template<typename fp>
constexpr fp poly_coeffs<fp>::a5;
template<typename fp>
constexpr fp poly_coeffs<fp>::a6;
template<typename fp>
constexpr fp poly_coeffs<fp>::a7;


template<typename Rep, int Exponent>
constexpr inline fixed_point<Rep, Exponent> evaluate_polynomial(fixed_point<Rep, Exponent> xf) {
	//Use a polynomial min-max approximation to generate the exponential of
	//the fractional part. Note that the constant 1 of the polynomial is added later,
	//this gives us one more bit of precision here for free
	using coeffs = poly_coeffs<fixed_point<Rep, Exponent>>;
	return xf * (coeffs::a1 + xf * (coeffs::a2 + xf * (coeffs::a3 + xf * (coeffs::a4 + xf * (coeffs::a5 + xf *(coeffs::a6 + xf * coeffs::a7)) ) )));
}

//Computes 2^x - 1 for a number x between 0 and 1, strictly less than 1
//If the exponent is not negative, there is no fractional part,
//so this is always zero
template<typename Rep, int Exponent,
			typename std::enable_if<(Exponent >= 0), int>::type dummy = 0>
inline constexpr make_largest_ufraction<fixed_point<Rep, Exponent>> exp2m1_0to1(fixed_point<Rep, Exponent>) {
	return make_largest_ufraction<fixed_point<Rep, Exponent>>::from_data(0); //Cannot construct from 0, since that would be a shift by more than width of type!
}
//for a positive exponent, some work needs to be done
template<typename Rep, int Exponent
 ,	typename std::enable_if<(Exponent < 0), int>::type dummy = 0
>
constexpr inline make_largest_ufraction<fixed_point<Rep, Exponent>>
exp2m1_0to1(fixed_point<Rep, Exponent> x) {

	//Build the type with the same number of bits, all fractional,
	//and unsigned. That should be enough to exactly hold enough bits
	//to guarantee bit-accurate results
	using im = make_largest_ufraction<fixed_point<Rep, Exponent>>; //The intermediate value type

	return evaluate_polynomial(im{x}); //Important: convert the type once, to keep every multiply from costing a cast
}

template<typename Rep, int Exponent>
constexpr inline  Rep floor(fixed_point<Rep, Exponent> x) {
	return Rep{(x.data() ) >> -Exponent};
}

}// namespace _fixed_point_impl

template<typename Rep, int Exponent>
constexpr fixed_point<Rep, Exponent> exp2(fixed_point<Rep, Exponent> x) {
	using namespace _fixed_point_impl;

	using fp = fixed_point<Rep, Exponent>; // The input type
	using im = make_largest_ufraction<fp>;

	//Calculate the final result by shifting the fractional part around.
	//Remember to add the 1 which is left out to get 1 bit more resolution
	return floor(x) <= Exponent ?
			fp::from_data(1)	//return immediately if the shift would result in all bits being shifted out
		:	fp::from_data((		//Do the shifts manually. Once the branch with shift operators is merged, could use those
			(exp2m1_0to1<Rep, Exponent>(x - floor(x))).data()	//Calculate the exponent of the fractional part
						   >> (-im::exponent + Exponent - floor(x)))	//shift it to the right place
			          + (Rep{1} << (floor(x) - Exponent))); //The constant term must be one, to make integer powers correct

}

}



#endif /* FIXED_POINT_MATH_H_ */
