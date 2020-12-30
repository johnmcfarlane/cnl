
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_REMOVE_SIGNEDNESS_H)
#define CNL_IMPL_DUPLEX_INTEGER_REMOVE_SIGNEDNESS_H

#include "../type_traits/remove_signedness.h"
#include "forward_declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct remove_signedness<_impl::duplex_integer<Upper, Lower>>
        : std::type_identity<_impl::duplex_integer<remove_signedness_t<Upper>, Lower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_REMOVE_SIGNEDNESS_H
