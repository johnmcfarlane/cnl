
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_POWER_H)
#define CNL_IMPL_POWER_H 1

#include "../constant.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // power - one integer raised to another as a floating-point or integer type

        template<
                typename S, int Exponent, int Radix,
                bool PositiveExponent = (0<Exponent),
                bool OddExponent = ((Exponent & 1)!=0),
                bool FloatingPointS = numeric_limits<S>::is_iec559>
        struct default_power;

        template<typename S, int Radix>
        struct default_power<S, 0, Radix, false, false, false> {
            constexpr S operator()() const
            {
                return S{1};
            }
        };

        template<typename S, int Exponent, bool OddExponent>
        struct default_power<S, Exponent, 2, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(S{1} << constant<Exponent>{})
            {
                // This check is intended to catch situations where a number is generated
                // which cannot possibly fit in the given type, S.
                // This may be caused by a conversion to or from a fixed_point type
                // which cannot possible store a non-zero value.
                using result_numeric_limits = numeric_limits<decltype(S{1} << constant<Exponent>{})>;
                static_assert(!std::is_integral<S>::value
                        || !std::is_signed<S>::value
                        || Exponent<result_numeric_limits::digits, "attempted operation will result in overflow");

                return S{1} << constant<Exponent>{};
            }
        };

        template<typename S, int Exponent, int Radix, bool OddExponent>
        struct default_power<S, Exponent, Radix, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(default_power<S, (Exponent-1), Radix>{}()*Radix)
            {
                return default_power<S, (Exponent-1), Radix>{}()*Radix;
            }
        };

        template<typename S, int Exponent, int Radix, bool PositiveExponent, bool OddExponent>
        struct default_power<S, Exponent, Radix, PositiveExponent, OddExponent, true> {
            constexpr S operator()() const
            {
                return Exponent
                       ? S(1.)/default_power<S, -Exponent, Radix>{}()
                       : S{1.};
            }
        };

        template<typename S, int Exponent, int Radix>
        struct default_power<S, Exponent, Radix, true, false, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }

            constexpr S operator()() const
            {
                return square(default_power<S, Exponent/2, Radix>{}());
            }
        };

        template<typename S, int Exponent, int Radix>
        struct default_power<S, Exponent, Radix, true, true, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }

            constexpr S operator()() const
            {
                return S(Radix)*default_power<S, (Exponent-1), Radix>{}();
            }
        };

        template<typename S, int Exponent, int Radix, class Enable = void>
        struct power {
            constexpr auto operator()() const
            -> decltype(default_power<S, Exponent, Radix>{}()) {
                return default_power<S, Exponent, Radix>{}();
            }
        };
    }

    template<typename S, int Exponent, int Radix>
    constexpr auto power()
    -> decltype(_impl::power<S, Exponent, Radix>{}())
    {
        return _impl::power<S, Exponent, Radix>{}();
    }
}

#endif  // CNL_IMPL_POWER_H
