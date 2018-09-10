
//          Copyright Timo Alho 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief some cmath specializations for `cnl::fixed_point` type;

#ifndef FIXED_POINT_MATH_H_
#define FIXED_POINT_MATH_H_

#include "type.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {

            template<class FixedPoint>
            constexpr FixedPoint rounding_conversion(double d) {
                using one_longer = fixed_point<set_digits_t<typename FixedPoint::rep, digits<FixedPoint>::value+1>, FixedPoint::exponent-1>;
                return from_rep<FixedPoint>(static_cast<typename FixedPoint::rep>((_impl::to_rep(one_longer{ d }) + 1) >> 1));
            }

            template<class FixedPoint>
            using unsigned_rep = typename std::make_unsigned<typename FixedPoint::rep>::type;

            template<class Input>
            using make_largest_ufraction = fixed_point<unsigned_rep<Input>, -digits<unsigned_rep<Input>>::value>;

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

            template<typename A, typename B>
            constexpr auto safe_multiply(A const& a, B const& b)
            -> enable_if_t<digits<decltype(a*b)>::value <= digits<A>::value+digits<B>::value,
                    decltype(set_digits_t<A, digits<A>::value+digits<B>::value>{a}
                            *set_digits_t<B, digits<A>::value+digits<B>::value>{b})> {
                return set_digits_t<A, digits<A>::value+digits<B>::value>{a}
                        *set_digits_t<B, digits<A>::value+digits<B>::value>{b};
            }

            template<typename A, typename B>
            constexpr auto safe_multiply(A const& a, B const& b)
            -> enable_if_t<digits<A>::value+digits<B>::value <= digits<decltype(a*b)>::value, decltype(a*b)> {
                return a*b;
            }

            template<class Rep, int Exponent>
            constexpr inline fixed_point<Rep, Exponent> evaluate_polynomial(
                    fixed_point<Rep, Exponent> xf) {
                using fp = fixed_point<Rep, Exponent>;

                //Use a polynomial min-max approximation to generate the exponential of
                //the fraction part. Note that the constant 1 of the polynomial is added later,
                //this gives us one more bit of precision here for free
                using coeffs = poly_coeffs<fp>;
                return fp{
                    safe_multiply(xf, (coeffs::a1+fp{
                        safe_multiply(xf, (coeffs::a2+fp{
                            safe_multiply(xf, (coeffs::a3+fp{
                                safe_multiply(xf, (coeffs::a4+fp{
                                    safe_multiply(xf, (coeffs::a5+fp{
                                        safe_multiply(xf, (coeffs::a6+fp{
                                            safe_multiply(coeffs::a7, xf)
                                        }))
                                    }))
                                }))
                            }))
                        }))
                    }))
                };
            }

            //Computes 2^x - 1 for a number x between 0 and 1, strictly less than 1
            //If the exponent is not negative, there is no fraction part,
            //so this is always zero
            template<class Rep, int Exponent>
            inline constexpr auto exp2m1_0to1(fixed_point<Rep, Exponent>)
            -> _impl::enable_if_t<(Exponent>=0), make_largest_ufraction<fixed_point<Rep, Exponent>>> {
                // Cannot construct from 0, since that would be a shift by more than width of type!
                return from_rep<make_largest_ufraction<fixed_point<Rep, Exponent>>>(0);
            }

            //for a positive exponent, some work needs to be done
            template<class Rep, int Exponent>
            constexpr inline auto exp2m1_0to1(fixed_point<Rep, Exponent> x)
            -> _impl::enable_if_t<(Exponent<0), make_largest_ufraction<fixed_point<Rep, Exponent>>> {
                //Build the type with the same number of bits, all fraction,
                //and unsigned. That should be enough to exactly hold enough bits
                //to guarantee bit-accurate results
                using im = make_largest_ufraction<fixed_point<Rep, Exponent>>;
                //The intermediate value type

                return evaluate_polynomial(im{fixed_point<typename im::rep, Exponent>{x}});
            }

            template<class Rep, int Exponent, int Radix>
            constexpr enable_if_t<-digits<Rep>::value<Exponent, fixed_point<Rep, Exponent, Radix>>
            fractional(fixed_point<Rep, Exponent, Radix> const& x, Rep const& floored) {
                return x - floored;
            }

            template<class Rep, int Exponent, int Radix>
            constexpr enable_if_t<-digits<Rep>::value>=Exponent, fixed_point<Rep, Exponent, Radix>>
            fractional(fixed_point<Rep, Exponent, Radix> const& x, Rep const&) {
                return x;
            }

            template<class Intermediate, typename Rep, int Exponent>
            constexpr typename Intermediate::rep
                    exp2(fixed_point<Rep, Exponent> const& x, Rep const& floored) {
                return floored <= Exponent
                    ? typename Intermediate::rep{1}//return immediately if the shift would result in all bits being shifted out
                    //Do the shifts manually. Once the branch with shift operators is merged, could use those
                    : (_impl::to_rep(exp2m1_0to1<Rep, Exponent>(fractional(x, floored)))//Calculate the exponent of the fraction part
                    >> (-Intermediate::exponent + Exponent - floored))//shift it to the right place
                    + (Rep { 1 } << (floored - Exponent)); //The constant term must be one, to make integer powers correct
            }
        }
    }

    /// Calculates exp2(x), i.e. 2^x
    /// \headerfile cnl/fixed_point.h
    ///
    /// Accurate to 1LSB for up to 32 bit underlying representation.
    ///
    /// \tparam x the input value as a fixed_point
    ///
    /// \return the result of the exponential, in the same representation as x
    template<class Rep, int Exponent>
    constexpr fixed_point<Rep, Exponent> exp2(fixed_point<Rep, Exponent> x) {
        using out_type = fixed_point<Rep, Exponent>;
        // The input type
        using im = _impl::fp::make_largest_ufraction<out_type>;

        //Calculate the final result by shifting the fraction part around.
        //Remember to add the 1 which is left out to get 1 bit more resolution
        return _impl::from_rep<out_type>(_impl::fp::exp2<im>(x, static_cast<Rep>(floor(x))));
    }

}

#endif /* FIXED_POINT_MATH_H_ */
