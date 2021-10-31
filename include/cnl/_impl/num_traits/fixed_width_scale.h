
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE_H)
#define CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE_H

#include "digits.h"
#include "from_rep.h"
#include "is_composite.h"
#include "scale.h"
#include "to_rep.h"

namespace cnl {
    // returns a scaled value of the same type
    template<int Digits, int Radix, class Scalar>
    struct fixed_width_scale;

    template<int Digits, int Radix, typename S>
    requires(!is_composite<S>::value) struct fixed_width_scale<Digits, Radix, S> {
        [[nodiscard]] constexpr auto operator()(S const& s) const
        {
            static_assert(
                    Radix != 2 || digits_v<S> > -Digits,
                    "this operation will flush the given value");

            return static_cast<S>(scale<Digits, Radix, S>()(s));
        }
    };

    namespace _impl {
        template<int Digits, int Radix = 2, class S>
        [[nodiscard]] constexpr auto fixed_width_scale(S const& s)
        {
            return cnl::fixed_width_scale<Digits, Radix, S>()(s);
        }
    }

    template<int Digits, int Radix, typename Composite>
    requires is_composite_v<Composite>
    struct fixed_width_scale<
            Digits, Radix, Composite> {
        [[nodiscard]] constexpr auto operator()(Composite const& s) const
        {
            return _impl::from_rep<Composite>(
                    _impl::fixed_width_scale<Digits, Radix>(_impl::to_rep(s)));
        }
    };
}

#endif  // CNL_IMPL_NUM_TRAITS_FIXED_WIDTH_SCALE_H
