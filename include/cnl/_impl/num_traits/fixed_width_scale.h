
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE
#define CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE

#include "digits.h"
#include "scale.h"

namespace cnl {
    // returns a scaled value of the same type
    template<int Digits, int Radix, class S, class Enable=void>
    struct fixed_width_scale {
        constexpr S operator()(S const& s) const
        {
            static_assert(
                    Radix!=2||digits<S>::value>-Digits,
                    "this operation will flush the given value");

            return static_cast<S>(scale<Digits, Radix, S>()(s));
        }
    };

    namespace _impl {
        template<int Digits, class S=void>
        constexpr S fixed_width_scale(S const& s)
        {
            return cnl::fixed_width_scale<Digits, numeric_limits<S>::radix, S>()(s);
        }
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE
