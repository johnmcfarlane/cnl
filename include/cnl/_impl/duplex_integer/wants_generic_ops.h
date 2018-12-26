
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_WANTS_GENERIC_OPS_H)
#define CNL_IMPL_DUPLEX_INTEGER_WANTS_GENERIC_OPS_H 1

#include "forward_declaration.h"
#include "../operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        struct wants_generic_ops<duplex_integer<Upper, Lower>> : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_WANTS_GENERIC_OPS_H
