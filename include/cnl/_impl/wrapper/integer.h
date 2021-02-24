
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_INTEGER_H)
#define CNL_IMPL_WRAPPER_INTEGER_H

#include "../../integer.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, typename Tag>
    struct is_integer<_impl::wrapper<Rep, Tag>> : std::true_type {
    };

    template<typename Rep, typename Tag>
    struct is_integer<_impl::wrapper<Rep, Tag> const> : std::true_type {
    };
}

#endif  // CNL_IMPL_WRAPPER_INTEGER_H
