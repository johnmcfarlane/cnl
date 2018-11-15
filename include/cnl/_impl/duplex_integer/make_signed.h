
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MAKE_SIGNED_H)
#define CNL_IMPL_DUPLEX_INTEGER_MAKE_SIGNED_H

#include "forward_declaration.h"
#include "../type_traits/make_signed.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct make_signed<_impl::duplex_integer<Upper, Lower>>
            : _impl::type_identity<_impl::duplex_integer<make_signed_t<Upper>, Lower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MAKE_SIGNED_H
