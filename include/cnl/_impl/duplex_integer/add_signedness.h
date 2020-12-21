
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_ADD_SIGNEDNESS_H)
#define CNL_IMPL_DUPLEX_INTEGER_ADD_SIGNEDNESS_H

#include "../type_traits/add_signedness.h"
#include "../type_traits/type_identity.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct add_signedness<_impl::duplex_integer<Upper, Lower>>
        : _impl::type_identity<_impl::duplex_integer<add_signedness_t<Upper>, Lower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_ADD_SIGNEDNESS_H
