
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_ROUNDING_H)
#define CNL_IMPL_DUPLEX_INTEGER_ROUNDING_H

#include "forward_declaration.h"
#include "../num_traits/rounding.h"
#include "../type_traits/type_identity.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct rounding<_impl::duplex_integer<Upper, Lower>>
            : _impl::type_identity<native_rounding_tag> {
        static_assert(
                std::is_same<rounding_t<Upper>, native_rounding_tag>::value,
                "This type can only be specialized with integers that have int-like rounding behavior.");
        static_assert(
                std::is_same<rounding_t<Lower>, native_rounding_tag>::value,
                "This type can only be specialized with integers that have int-like rounding behavior.");
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_ROUNDING_H
