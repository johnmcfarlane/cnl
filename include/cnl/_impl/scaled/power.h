
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_SCALED_POWER_H
#define CNL_IMPL_SCALED_POWER_H

#include "../operators/is_same_tag_family.h"

/// compositional numeric library
namespace cnl {
    template<int Exponent = 0, int Radix = 2>
    struct power {
        static_assert(Radix >= 2, "Radix must be two or greater");

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// value of template parameter, \a Radix
        constexpr static int radix = Radix;

        ////////////////////////////////////////////////////////////////////////////////
        // types

        using identity = power<0, Radix>;
    };

    /// value of template parameter, \a Exponent
    template<int Exponent, int Radix>
    constexpr int power<Exponent, Radix>::exponent;

    namespace _impl {
        template<int Exponent1, int Radix1, int Exponent2, int Radix2>
        struct is_same_tag_family<power<Exponent1, Radix1>, power<Exponent2, Radix2>> : std::true_type {
        };
    }
}

#endif //CNL_IMPL_SCALED_POWER_H
