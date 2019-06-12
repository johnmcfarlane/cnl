
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_SCALE_H)
#define CNL_IMPL_DUPLEX_INTEGER_SCALE_H

#include "forward_declaration.h"
#include "../num_traits/scale.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, int Radix, typename Upper, typename Lower>
    struct scale<Digits, Radix, _impl::duplex_integer<Upper, Lower>>
            : _impl::default_scale<Digits, Radix, _impl::duplex_integer<Upper, Lower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_SCALE_H
