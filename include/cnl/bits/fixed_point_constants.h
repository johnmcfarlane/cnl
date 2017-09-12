
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief fixed_point specializations of math constants from <cnl/numeric.h>

#ifndef FIXED_POINT_CONSTANTS_H_
#define FIXED_POINT_CONSTANTS_H_

#include "fixed_point_type.h"

/// compositional numeric library
namespace cnl {

#if defined(__cpp_inline_variables)

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of math constants from numeric.h

    namespace _impl {
        template<typename Rep, int Exponent>
        constexpr auto pi(int const max_iterations = 0) {
            using fp = fixed_point<Rep, Exponent>;
            constexpr auto four = fixed_point<Rep, 3 - digits<Rep>::value>{4};

            auto previous = fp{3};
            for(auto n = 2; n != (max_iterations << 1); n += 4) {
                auto const addend     = divide(four, ((n+0L) * (n+1L) * (n+2L)));
                auto const subtrahend = divide(four, ((n+2L) * (n+3L) * (n+4L)));
                auto next = fp{previous + addend - subtrahend};
                if (next == previous) {
                    return next;
                }
                previous = next;
            }
            return previous;
        }

        template<typename Rep, int Exponent>
        constexpr auto e(int const max_iterations = 0) {
            using fp = fixed_point<Rep, Exponent>;
            constexpr auto one = fixed_point<Rep, 1 - digits<Rep>::value>{1};

            auto previous = fp{2};
            auto factor = 2;
            for (auto n = 2; n != max_iterations; ++ n, factor *= n) {
                auto const addend = divide(one, factor);
                auto next = fp{previous + addend};
                if (next == previous) {
                    return next;
                }
                previous = next;
            }
            return previous;
        }

        // Given two alternative ways to generate a constant value:
        // tries to choose the best and returns the result.
        template<typename Float, typename Rep, int Exponent>
        constexpr auto constant_with_fallback(Float constant, fixed_point<Rep, Exponent>(*procedure)(int), int max_iterations = 0) {
            using fp = fixed_point<Rep, Exponent>;

            auto const whole = static_cast<int>(constant);
            auto const required_integer_digits = used_bits(static_cast<int>(constant));

            constexpr auto fixed_fractional_digits = fractional_digits<fp>::value;
            auto const fixed_digits = fixed_fractional_digits + required_integer_digits;
            // expect: integer_digits<fp>::value >= required_integer_digits

            constexpr auto float_digits = std::numeric_limits<Float>::digits;
            auto const float_fractional_digits = float_digits - required_integer_digits;

            return (float_fractional_digits >= fixed_fractional_digits)
                ? fp{constant}
                   : procedure(max_iterations);
        }
    }

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> e<fixed_point<Rep, Exponent>> {
        _impl::constant_with_fallback<long double, Rep, Exponent>(e<long double>, _impl::e<Rep, Exponent>)
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> log2e<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 1.442695040888963407359924681001892137L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> log10e<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 0.434294481903251827651128918916605082L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> ln2<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 0.693147180559945309417232121458176568L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> ln10<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 2.302585092994045684017991454684364208L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> pi<fixed_point<Rep, Exponent>>{
        _impl::constant_with_fallback<long double, Rep, Exponent>(pi<long double>, _impl::pi<Rep, Exponent>)
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> pi_2<fixed_point<Rep, Exponent>>{
        pi<fixed_point<Rep, Exponent+1>> >> std::integral_constant<int, 1>{}
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> pi_4<fixed_point<Rep, Exponent>>{
        pi<fixed_point<Rep, Exponent+2>> >> std::integral_constant<int, 2>{}
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> one_pi<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 0.318309886183790671537767526745028724L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> two_pi<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 0.636619772367581343075535053490057448L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> two_sqrtpi<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 1.128379167095512573896158903121545172L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> sqrt2<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 1.414213562373095048801688724209698079L }
    };

    template<typename Rep, int Exponent> inline constexpr fixed_point<Rep, Exponent> one_sqrt2<fixed_point<Rep, Exponent>> {
        fixed_point<Rep, Exponent>{ 0.707106781186547524400844362104849039L }
    };

#endif  // defined(__cpp_inline_variables)
}

#endif // FIXED_POINT_CONSTANTS_H_
