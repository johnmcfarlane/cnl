
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief some cmath specializations for `sg14::fixed_point` type;

#ifndef FIXED_POINT_MATH_H_
#define FIXED_POINT_MATH_H_

#include <sg14/fixed_point>

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {

            template<class FixedPoint>
            constexpr FixedPoint rounding_conversion(double d) {
                using one_longer = make_fixed<FixedPoint::integer_digits, FixedPoint::fractional_digits + 1>;
                return FixedPoint::from_data(static_cast<typename FixedPoint::rep>((one_longer{ d }.data() + 1) >> 1));
            }

            template<class FixedPoint>
            using unsigned_rep = typename std::make_unsigned<typename FixedPoint::rep>::type;

            template<class Input>
            using make_largest_ufraction = fixed_point<unsigned_rep<Input>, -std::numeric_limits<unsigned_rep<Input>>::digits>;

            static_assert(std::is_same<make_largest_ufraction<fixed_point<int32_t, -15>>, fixed_point<uint32_t, -32>>::value, "");

            //TODO: template magic to get the coefficients automatically
            //from the number of bits of precision
            //Define the coefficients as constexpr,
            //to make sure they're converted to fp
            //at compile time
            template<class CoeffType>
            struct poly_coeffs {
                static constexpr CoeffType a1 { rounding_conversion<CoeffType>(0.6931471860838825) };
                static constexpr CoeffType a2 { rounding_conversion<CoeffType>(0.2402263846181129) };
                static constexpr CoeffType a3 { rounding_conversion<CoeffType>(
                        0.055505126858894846) };
                static constexpr CoeffType a4 { rounding_conversion<CoeffType>(
                        0.009614017013719252) };
                static constexpr CoeffType a5 { rounding_conversion<CoeffType>(
                        0.0013422634797558564) };
                static constexpr CoeffType a6 { rounding_conversion<CoeffType>(
                        0.00014352314226313836) };
                static constexpr CoeffType a7 { rounding_conversion<CoeffType>(
                        0.000021498763160402416) };
            };

            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a1;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a2;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a3;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a4;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a5;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a6;
            template<class CoeffType>
            constexpr CoeffType poly_coeffs<CoeffType>::a7;

            template<class Rep, int Exponent>
            constexpr inline fixed_point<Rep, Exponent> evaluate_polynomial(
                    fixed_point<Rep, Exponent> xf) {
                using fp = fixed_point<Rep, Exponent>;

                //Use a polynomial min-max approximation to generate the exponential of
                //the fractional part. Note that the constant 1 of the polynomial is added later,
                //this gives us one more bit of precision here for free
                using coeffs = poly_coeffs<fp>;
                return fp{multiply(xf, (coeffs::a1+fp{multiply(xf, (coeffs::a2+fp{multiply(xf, (coeffs::a3+fp{multiply(xf, (coeffs::a4
                        +fp{multiply(xf, (coeffs::a5+fp{multiply(xf, (coeffs::a6+fp{multiply(fp{coeffs::a7}, fp{xf})}))}))}))}))}))}))};
            }

            //Computes 2^x - 1 for a number x between 0 and 1, strictly less than 1
            //If the exponent is not negative, there is no fractional part,
            //so this is always zero
            template<class Rep, int Exponent, _impl::enable_if_t<(Exponent>=0), int> dummy = 0>
            inline constexpr make_largest_ufraction<fixed_point<Rep, Exponent>> exp2m1_0to1(
                    fixed_point<Rep, Exponent>) {
                return make_largest_ufraction<fixed_point<Rep, Exponent>>::from_data(
                        0); //Cannot construct from 0, since that would be a shift by more than width of type!
            }
            //for a positive exponent, some work needs to be done
            template<class Rep, int Exponent, _impl::enable_if_t<(Exponent<0), int> dummy = 0>
            constexpr inline make_largest_ufraction<fixed_point<Rep, Exponent>> exp2m1_0to1(
                    fixed_point<Rep, Exponent> x) {

                //Build the type with the same number of bits, all fractional,
                //and unsigned. That should be enough to exactly hold enough bits
                //to guarantee bit-accurate results
                using im = make_largest_ufraction<fixed_point<Rep, Exponent>>;
                //The intermediate value type

                return evaluate_polynomial(im{x}); //Important: convert the type once, to keep every multiply from costing a cast
            }

            template<class Rep, int Exponent>
            constexpr inline Rep floor(fixed_point<Rep, Exponent> x) {
                return Rep { (x.data()) >> -Exponent };
            }

        }
    }

    /// Calculates exp2(x), i.e. 2^x
    /// \headerfile sg14/fixed_point
    ///
    /// Accurate to 1LSB for up to 32 bit underlying representation.
    ///
    /// \tparam x the input value as a fixed_point
    ///
    /// \return the result of the exponential, in the same representation as x
    template<class Rep, int Exponent>
    constexpr fixed_point<Rep, Exponent> exp2(fixed_point<Rep, Exponent> x) {
        using namespace _impl::fp;

        using out_type = fixed_point<Rep, Exponent>;
        // The input type
        using im = make_largest_ufraction<out_type>;

        //Calculate the final result by shifting the fractional part around.
        //Remember to add the 1 which is left out to get 1 bit more resolution
        return out_type::from_data(
                floor(x) <= Exponent ?
                    typename im::rep{1}//return immediately if the shift would result in all bits being shifted out
                                     :
                    	//Do the shifts manually. Once the branch with shift operators is merged, could use those
                    (exp2m1_0to1<Rep, Exponent>(static_cast<out_type>(x - floor(x))).data()//Calculate the exponent of the fractional part
                    >> (-im::exponent + Exponent - floor(x)))//shift it to the right place
                    + (Rep { 1 } << (floor(x) - Exponent))); //The constant term must be one, to make integer powers correct
    }

}

#endif /* FIXED_POINT_MATH_H_ */
