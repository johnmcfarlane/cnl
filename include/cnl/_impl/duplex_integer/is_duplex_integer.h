
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_IS_DUPLEX_INTEGER_H)
#define CNL_IMPL_DUPLEX_INTEGER_IS_DUPLEX_INTEGER_H

#include "forward_declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename T>
        struct is_duplex_integer : std::false_type {};

        template<typename Upper, typename Lower>
        struct is_duplex_integer<duplex_integer<Upper, Lower>> : std::true_type {};
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_IS_DUPLEX_INTEGER_H
