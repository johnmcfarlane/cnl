
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief scaled_integer specializations of math constants from <cnl/numeric.h>

#if !defined(CNL_IMPL_SCALED_INTEGER_CONSTANTS_H)
#define CNL_IMPL_SCALED_INTEGER_CONSTANTS_H

#include "num_traits.h"
#include "type.h"

#include "../../numbers.h"
#include "../used_digits.h"

/// compositional numeric library
namespace cnl {

#if defined(__cpp_inline_variables)

    ////////////////////////////////////////////////////////////////////////////////
    // scaled_integer specializations of math constants from numeric.h

    namespace _impl {
        template<typename Rep, int Exponent>
        CNL_NODISCARD constexpr auto pi(int const max_iterations) {
            using fp = scaled_integer<Rep, power<Exponent>>;
            constexpr auto four = scaled_integer<Rep, power<3 - digits_v<Rep>>>{4.};

            auto previous = fp{3.};
            for(auto n = 2; n != (max_iterations << 1); n += 4) {
                auto const addend     = four / ((n+0L) * (n+1L) * (n+2L));
                auto const subtrahend = four / ((n+2L) * (n+3L) * (n+4L));
                auto next = fp{previous + addend - subtrahend};
                if (next == previous) {
                    return next;
                }
                previous = next;
            }
            return previous;
        }

        template<typename Rep, int Exponent>
        CNL_NODISCARD constexpr auto pi() {
            return pi<Rep, Exponent>(0);
        }

        template<typename Rep, int Exponent>
        CNL_NODISCARD constexpr auto e(int const max_iterations) {
            using fp = scaled_integer<Rep, power<Exponent>>;
            constexpr auto one = scaled_integer<Rep, power<2 - digits_v<Rep>>>{1.};

            auto previous = fp{2.};
            auto factor = 2;
            for (auto n = 2; n != max_iterations; ++ n, factor *= n) {
                auto const addend = one / factor;
                auto next = fp{previous + addend};
                if (next == previous) {
                    return next;
                }
                previous = next;
            }
            return previous;
        }

        template<typename Rep, int Exponent>
        CNL_NODISCARD constexpr auto e() {
            return e<Rep, Exponent>(0);
        }

        // Given two alternative ways to generate a constant value:
        // tries to choose the best and returns the result.
        template<typename Float, typename Rep, int Exponent>
        CNL_NODISCARD constexpr auto constant_with_fallback(Float constant, scaled_integer<Rep, power<Exponent>>(*procedure)()) {
            using fp = scaled_integer<Rep, power<Exponent>>;

            auto const required_integer_digits = used_digits(static_cast<int>(constant));

            constexpr auto fixed_fractional_digits = fractional_digits<fp>::value;
            // expect: integer_digits<fp>::value >= required_integer_digits

            constexpr auto float_digits = std::numeric_limits<Float>::digits;
            auto const float_fractional_digits = float_digits - required_integer_digits;

            return (float_fractional_digits >= fixed_fractional_digits)
                ? fp{constant}
                   : procedure();
        }
    }

    namespace numbers {
        /// specialization of \ref cnl::e for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> e_v<scaled_integer<Rep, power<Exponent>>> {
                _impl::constant_with_fallback<long double, Rep, Exponent>(e_v<long double>, _impl::e<Rep, Exponent>)
        };

        /// specialization of \ref cnl::log2e for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> log2e_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{log2e_v<long double>}
        };

        /// specialization of \ref cnl::log10e for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> log10e_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{log10e_v<long double>}
        };

        /// specialization of \ref cnl::pi for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> pi_v<scaled_integer<Rep, power<Exponent>>>{
                _impl::constant_with_fallback<long double, Rep, Exponent>(pi_v<long double>, _impl::pi<Rep, Exponent>)
        };

        /// specialization of \ref cnl::invpi for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> inv_pi_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{inv_pi_v<long double>}
        };

        /// specialization of \ref cnl::inv_sqrtpi for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> inv_sqrtpi_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{inv_sqrtpi_v<long double>}
        };

        /// specialization of \ref cnl::ln2 for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> ln2_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{ln2_v<long double>}
        };

        /// specialization of \ref cnl::ln10 for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> ln10_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{ln10_v<long double>}
        };

        /// specialization of \ref cnl::sqrt2 for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> sqrt2_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{sqrt2_v<long double>}
        };

        /// specialization of \ref cnl::sqrt3 for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> sqrt3_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{sqrt3_v<long double>}
        };

        /// specialization of \ref cnl::invsqrt3 for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> inv_sqrt3_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{inv_sqrt3_v<long double>}
        };

        /// specialization of \ref cnl::egamma for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> egamma_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{egamma_v<long double>}
        };

        /// specialization of \ref cnl::phi for \ref cnl::scaled_integer
        template<typename Rep, int Exponent> inline constexpr scaled_integer<Rep, power<Exponent>> phi_v<scaled_integer<Rep, power<Exponent>>> {
                scaled_integer<Rep, power<Exponent>>{phi_v<long double>}
        };
    }
#endif  // defined(__cpp_inline_variables)
}

#endif // CNL_IMPL_SCALED_INTEGER_CONSTANTS_H
