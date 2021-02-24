
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_NUMBERS_H)
#define CNL_IMPL_DUPLEX_INTEGER_NUMBERS_H

#include "../numbers/set_signedness.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library, numbers header/namespace
namespace cnl::numbers {
    template<typename Upper, typename Lower>
    struct signedness<_impl::duplex_integer<Upper, Lower>> : signedness<Upper> {
    };

    template<typename Upper, typename Lower, bool IsSigned>
    struct set_signedness<_impl::duplex_integer<Upper, Lower>, IsSigned>
        : std::type_identity<_impl::duplex_integer<numbers::set_signedness_t<Upper, IsSigned>, Lower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_NUMBERS_H
