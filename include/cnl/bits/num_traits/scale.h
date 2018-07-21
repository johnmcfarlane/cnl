
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_BITS_NUM_TRAITS_SCALE
#define CNL_BITS_NUM_TRAITS_SCALE

#include "shift.h"

namespace cnl {
    // returns a scaled value of the same type
    template<int Digits, int Radix, class S, class Enable=void>
    struct scale {
        constexpr S operator()(S const& s) const
        {
            return static_cast<S>(shift<Digits, Radix, S>()(s));
        }
    };

    namespace _impl {
        template<int Digits, class S=void>
        constexpr S scale(S const& s)
        {
            return cnl::scale<Digits, numeric_limits<S>::radix, S>()(s);
        }
    }
}

#endif  // CNL_BITS_NUM_TRAITS_SCALE
