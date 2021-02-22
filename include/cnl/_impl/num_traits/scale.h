
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_SCALE)
#define CNL_IMPL_NUM_TRAITS_SCALE

#include "../power_value.h"
#include "../type_traits/is_integral.h"

namespace cnl {
    template<int Digits, int Radix, class S>
    struct scale;

    namespace _impl {
        // fundamental integer-friendly cnl::scale algorithm
        template<int Digits, int Radix, typename S>
        struct default_scale;

        template<int Digits, int Radix, typename S>
        requires(0 <= Digits) struct default_scale<Digits, Radix, S> {
            [[nodiscard]] constexpr auto operator()(S const& s) const
            {
                return s * power_value<S, Digits, Radix>();
            }
        };

        // cnl::default_scale<-ve, cnl::constant<>>
        template<int Digits, int Radix, typename S>
        requires(Digits < 0) struct default_scale<Digits, Radix, S> {
            [[nodiscard]] constexpr auto operator()(S const& s) const
            {
                return s / power_value<S, -Digits, Radix>();
            }
        };
    }

    // cnl::scale<..., fundamental-integer>
    template<int Digits, int Radix, _impl::integral S>
    struct scale<Digits, Radix, S>
        : _impl::default_scale<Digits, Radix, S> {
    };

    namespace _impl {
        // cnl::_impl::scale - convenience wrapper for cnl::scale
        template<int Digits, int Radix = 2, class S>
        [[nodiscard]] constexpr auto scale(S const& s)
        {
            return cnl::scale<Digits, Radix, S>{}(s);
        }
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_SCALE
