
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_CTORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_CTORS_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        constexpr int duplex_integer<Upper, Lower>::lower_width;

        template<typename Upper, typename Lower>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(upper_type const& u, lower_type const& l)
                :_upper(u), _lower(l) { }

        template<typename Upper, typename Lower>
        template<typename Integer, _impl::enable_if_t<(numeric_limits<Integer>::is_integer), int> Dummy>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Integer const& i)
                : _upper(static_cast<upper_type>(
                        (lower_width<digits<Integer>::value)
                        ? sensible_right_shift(i, lower_width)
                        : (i >> (digits<Integer>::value-1)) >> 1)),
                  _lower(static_cast<lower_type>(lower_type(i) & numeric_limits<lower_type>::max()))
        {
        }
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_CTORS_H
