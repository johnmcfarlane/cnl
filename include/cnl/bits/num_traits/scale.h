
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_BITS_NUM_TRAITS_SCALE
#define CNL_BITS_NUM_TRAITS_SCALE

#include <cnl/bits/power.h>
#include <cnl/bits/type_traits.h>
#include <cnl/bits/type_traits/is_integral.h>

namespace cnl {
    template<int Digits, int Radix, class S, class Enable = void>
    struct scale;

    namespace _impl {
        // fundamental integer-friendly cnl::scale algorithm
        template<int Digits, int Radix, typename S, class Enable = void>
        struct default_scale;

        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<0<=Digits>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s*power<S, Digits, Radix>())
            {
                return s*power<S, Digits, Radix>();
            }
        };

        // cnl::default_scale<-ve, cnl::constant<>>
        template<int Digits, int Radix, typename S>
        struct default_scale<Digits, Radix, S, _impl::enable_if_t<Digits<0>> {
            constexpr auto operator()(S const& s) const
            -> decltype(s/power<S, -Digits, Radix>())
            {
                return s/power<S, -Digits, Radix>();
            }
        };
    }

    // cnl::scale<..., fundamental-integer>
    template<int Digits, int Radix, class S>
    struct scale<Digits, Radix, S, _impl::enable_if_t<cnl::_impl::is_integral<S>::value>>
            : _impl::default_scale<Digits, Radix, S> {
    };

    namespace _impl {
        // cnl::_impl::scale - convenience wrapper for cnl::scale
        template<int Digits, int Radix=2, class S>
        constexpr auto scale(S const& s)
        -> decltype(cnl::scale<Digits, Radix, S>{}(s))
        {
            return cnl::scale<Digits, Radix, S>{}(s);
        }
    }
}

#endif  // CNL_BITS_NUM_TRAITS_SCALE
