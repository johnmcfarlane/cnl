
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_IS_SIGNED_H)
#define CNL_IMPL_DUPLEX_INTEGER_IS_SIGNED_H

#include "forward_declaration.h"
#include "../type_traits/is_signed.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct is_signed<_impl::duplex_integer<Upper, Lower>>
            : std::integral_constant<bool, is_signed<Upper>::value> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_IS_SIGNED_H
