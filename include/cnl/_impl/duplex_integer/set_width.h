
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_SET_WIDTH_H)
#define CNL_IMPL_DUPLEX_INTEGER_SET_WIDTH_H

#include "instantiate_duplex_integer.h"
#include "../num_traits/width.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower, int Digits>
        struct set_width<_impl::duplex_integer<Upper, Lower>, Digits> {
            using word = _impl::to_rep_t<Upper>;
            using type = typename _impl::instantiate_duplex_integer<Digits+!is_signed<word>::value, word>::type;
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_SET_WIDTH_H
