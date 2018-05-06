
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_POWER_H)
#define CNL_POWER_H 1

#include <cnl/constant.h>

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
        struct _power;

        template<typename S>
        struct _power<S, 0, 2, false, false, false> {
            constexpr int operator()() const
            {
                return 1;
            }
        };

        template<typename S, int Radix>
        struct _power<S, 0, Radix, false, false, false> {
            constexpr S operator()() const
            {
                return 1;
            }
        };

        template<typename S, int Exponent, bool OddExponent>
        struct _power<S, Exponent, 2, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(S{1} << constant<Exponent>{})
            {
                return S{1} << constant<Exponent>{};
            }
        };

        template<typename S, int Exponent, int Radix, bool OddExponent>
        struct _power<S, Exponent, Radix, true, OddExponent, false> {
            constexpr auto operator()() const
            -> decltype(_power<S, (Exponent-1), Radix>{}()*Radix)
            {
                return _power<S, (Exponent-1), Radix>{}()*Radix;
            }
        };

        template<typename S, int Exponent, int Radix, bool PositiveExponent, bool OddExponent>
        struct _power<S, Exponent, Radix, PositiveExponent, OddExponent, true> {
            constexpr S operator()() const
            {
                return Exponent
                       ? S(1.)/_power<S, -Exponent, Radix>{}()
                       : S{1.};
            }
        };

        template<typename S, int Exponent, int Radix>
        struct _power<S, Exponent, Radix, true, false, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }

            constexpr S operator()() const
            {
                return square(_power<S, Exponent/2, Radix>{}());
            }
        };

        template<typename S, int Exponent, int Radix>
        struct _power<S, Exponent, Radix, true, true, true> {
            constexpr static S square(S const& r)
            {
                return r*r;
            }

            constexpr S operator()() const
            {
                return S(Radix)*_power<S, (Exponent-1), Radix>{}();
            }
        };

        template<typename S, int Exponent, int Radix>
        constexpr auto power()
        -> decltype(_power<S, Exponent, Radix>{}())
        {
            return _power<S, Exponent, Radix>{}();
        }
    }
}

#endif  // CNL_FIXED_POINT_DEF_H
