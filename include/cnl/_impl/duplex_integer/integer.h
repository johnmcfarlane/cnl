
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_INTEGER_H)
#define CNL_IMPL_DUPLEX_INTEGER_INTEGER_H

#include "../../integer.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct is_integer<_impl::duplex_integer<Upper, Lower>> : std::true_type {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_INTEGER_H
