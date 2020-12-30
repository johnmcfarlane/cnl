
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_REP_OF_H)
#define CNL_IMPL_DUPLEX_INTEGER_REP_OF_H

#include "../num_traits/to_rep.h"
#include "forward_declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct rep_of<_impl::duplex_integer<Upper, Lower>> : std::type_identity<Upper> {
    };

    template<typename UpperUpper, typename UpperLower, typename Lower>
    struct rep_of<_impl::duplex_integer<_impl::duplex_integer<UpperUpper, UpperLower>, Lower>>
        : rep_of<_impl::duplex_integer<UpperUpper, UpperLower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_REP_OF_H
