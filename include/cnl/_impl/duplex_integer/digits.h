
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_DIGITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_DIGITS_H

#include "forward_declaration.h"
#include "../num_traits/digits.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Lower, typename Upper>
    struct digits<_impl::duplex_integer<Lower, Upper>>
            : std::integral_constant<int, digits<Lower>::value+digits<Upper>::value> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_DIGITS_H
