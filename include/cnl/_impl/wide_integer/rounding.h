
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_ROUNDING_H)
#define CNL_IMPL_WIDE_INTEGER_ROUNDING_H

#include "forward_declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    struct rounding<_impl::wide_integer<Digits, Narrowest>>
            : rounding<Narrowest> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_ROUNDING_H
