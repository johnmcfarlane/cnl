
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_POWER_H)
#define CNL_IMPL_POWER_H

#include "../constant.h"
#include "narrow_cast.h"
#include "num_traits/digits.h"
#include "num_traits/from_value.h"

#include <limits>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // power_value - one integer raised to another as a floating-point or integer type

        template<
                typename S, int Exponent, int Radix, bool PositiveExponent = (0 < Exponent),
                bool OddExponent = ((Exponent & 1) != 0),
                bool FloatingPointS = std::numeric_limits<S>::is_iec559>
        struct power_value_fn;

        template<typename S, int Radix>
        struct power_value_fn<S, 0, Radix, false, false, false> {
            [[nodiscard]] constexpr auto operator()() const
            {
                return S{1};
            }
        };

        template<typename S, int Exponent, bool OddExponent>
        struct power_value_fn<S, Exponent, 2, true, OddExponent, false> {
            [[nodiscard]] constexpr auto operator()() const
            {
                using result_numeric_limits = std::numeric_limits<decltype(
                        decltype(std::declval<S>() >> std::declval<constant<digits_v<S> - 1>>()){1}
                        << constant<Exponent>{})>;
                static_assert(
                        !std::is_integral<S>::value || !std::is_signed<S>::value
                                || Exponent < result_numeric_limits::digits,
                        "attempted operation will result in overflow");

                // TODO: This expression is so ugly that it might justify
                // a separate specialization of power for elastic_integer
                return decltype(std::declval<S>() >> std::declval<constant<digits_v<S> - 1>>()){1}
                    << constant<Exponent>{};
            }
        };

        template<typename S, int Exponent, int Radix, bool OddExponent>
        struct power_value_fn<S, Exponent, Radix, true, OddExponent, false> {
            [[nodiscard]] constexpr auto operator()() const
            {
                return power_value_fn<S, (Exponent - 1), Radix>{}() * Radix;
            }
        };

        template<typename S, int Exponent, int Radix, bool PositiveExponent, bool OddExponent>
        struct power_value_fn<S, Exponent, Radix, PositiveExponent, OddExponent, true> {
            [[nodiscard]] constexpr auto operator()() const -> S
            {
                return Exponent ? S{1.} / power_value_fn<S, -Exponent, Radix>{}() : S{1.};
            }
        };

        template<typename S, int Exponent, int Radix>
        struct power_value_fn<S, Exponent, Radix, true, false, true> {
            [[nodiscard]] constexpr static auto square(S const& r)
            {
                return r * r;
            }

            [[nodiscard]] constexpr auto operator()() const
            {
                return square(power_value_fn<S, Exponent / 2, Radix>{}());
            }
        };

        template<typename S, int Exponent, int Radix>
        struct power_value_fn<S, Exponent, Radix, true, true, true> {
            [[nodiscard]] constexpr static auto square(S const& r)
            {
                return r * r;
            }

            [[nodiscard]] constexpr auto operator()() const
            {
                return narrow_cast<S>(Radix) * power_value_fn<S, (Exponent - 1), Radix>{}();
            }
        };

        template<typename S, int Exponent, int Radix>
        [[nodiscard]] constexpr auto power_value()
        {
            return power_value_fn<S, Exponent, Radix>{}();
        }
    }
}

#endif  // CNL_IMPL_POWER_H
