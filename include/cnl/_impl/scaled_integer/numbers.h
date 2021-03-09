
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief scaled_integer specializations of math constants from `<numbers>`

#if !defined(CNL_IMPL_SCALED_INTEGER_NUMBERS_H)
#define CNL_IMPL_SCALED_INTEGER_NUMBERS_H

#include "../../limits.h"
#include "../used_digits.h"
#include "definition.h"
#include "num_traits.h"

#include <type_traits>
#include <version>

#if defined(__cpp_lib_math_constants)

#include <numbers>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep, int Exponent>
        [[nodiscard]] constexpr auto pi(int const max_iterations)
        {
            using fp = scaled_integer<Rep, power<Exponent>>;
            constexpr auto four = scaled_integer<Rep, power<3 - digits<Rep>>>{4.};

            auto previous = fp{3.};
            for (auto n = 2; n != (max_iterations << 1); n += 4) {
                auto const addend = four / ((n + 0L) * (n + 1L) * (n + 2L));
                auto const subtrahend = four / ((n + 2L) * (n + 3L) * (n + 4L));
                auto next = fp{previous + addend - subtrahend};
                if (next == previous) {
                    return next;
                }
                previous = next;
            }
            return previous;
        }

        template<typename Rep, int Exponent>
        [[nodiscard]] constexpr auto pi()
        {
            return pi<Rep, Exponent>(0);
        }

        template<typename Rep, int Exponent>
        [[nodiscard]] constexpr auto e(int const max_iterations)
        {
            using fp = scaled_integer<Rep, power<Exponent>>;
            constexpr auto one = scaled_integer<Rep, power<2 - digits<Rep>>>{1.};

            auto previous = fp{2.};
            auto factor = 2;
            for (auto n = 2; n != max_iterations; ++n, factor *= n) {
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
        [[nodiscard]] constexpr auto e()
        {
            return e<Rep, Exponent>(0);
        }

        // Given two alternative ways to generate a constant value:
        // tries to choose the best and returns the result.
        template<typename Float, typename Rep, int Exponent>
        [[nodiscard]] constexpr auto constant_with_fallback(
                // NOLINTNEXTLINE(misc-unused-parameters)
                Float constant, scaled_integer<Rep, power<Exponent>> (*procedure)())
        {
            using fp = scaled_integer<Rep, power<Exponent>>;

            auto const required_integer_digits = used_digits(static_cast<int>(constant));

            constexpr auto fixed_fractional_digits = fractional_digits<fp>;
            // expect: integer_digits<fp> >= required_integer_digits

            constexpr auto float_digits = cnl::numeric_limits<Float>::digits;
            auto const float_fractional_digits = float_digits - required_integer_digits;

            return (float_fractional_digits >= fixed_fractional_digits) ? fp{constant}
                                                                        : procedure();
        }
    }
}

// https://bugs.llvm.org/show_bug.cgi?id=46109
// NOLINTNEXTLINE(cert-dcl58-cpp)
namespace std {
    namespace numbers {
        /// \brief specialization of \ref std::numbers::e_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                e_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::_impl::constant_with_fallback<long double, Rep, Exponent>(
                                e_v<long double>, cnl::_impl::e<Rep, Exponent>)};

        /// \brief specialization of \ref std::numbers::log2e_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                log2e_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{log2e_v<long double>}};

        /// \brief specialization of \ref std::numbers::log10e_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                log10e_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{log10e_v<long double>}};

        /// \brief specialization of \ref std::numbers::pi_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                pi_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::_impl::constant_with_fallback<long double, Rep, Exponent>(
                                pi_v<long double>, cnl::_impl::pi<Rep, Exponent>)};

        /// \brief specialization of \ref std::numbers::inv_pi_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                inv_pi_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{inv_pi_v<long double>}};

        /// \brief specialization of \ref std::numbers::inv_sqrtpi_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                inv_sqrtpi_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{inv_sqrtpi_v<long double>}};

        /// \brief specialization of \ref std::numbers::ln2_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                ln2_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{ln2_v<long double>}};

        /// \brief specialization of \ref std::numbers::ln10_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                ln10_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{ln10_v<long double>}};

        /// \brief specialization of \ref std::numbers::sqrt2_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                sqrt2_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{sqrt2_v<long double>}};

        /// \brief specialization of \ref std::numbers::sqrt3_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                sqrt3_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{sqrt3_v<long double>}};

        /// \brief specialization of \ref std::numbers::inv_sqrt3_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                inv_sqrt3_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{inv_sqrt3_v<long double>}};

        /// \brief specialization of \ref std::numbers::egamma_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                egamma_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{egamma_v<long double>}};

        /// \brief specialization of \ref std::numbers::phi_v for \ref cnl::scaled_integer
        template<typename Rep, int Exponent>
        inline constexpr cnl::scaled_integer<Rep, cnl::power<Exponent>>
                phi_v<cnl::scaled_integer<Rep, cnl::power<Exponent>>>{
                        cnl::scaled_integer<Rep, cnl::power<Exponent>>{phi_v<long double>}};
    }
}

#endif  // __cpp_­lib_­math_­constants

#endif  // CNL_IMPL_SCALED_INTEGER_NUMBERS_H
