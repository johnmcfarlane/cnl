
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_REP_OF_H)
#define CNL_IMPL_WRAPPER_REP_OF_H

#include "../operators/tag.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Rep, tag Tag>
    struct rep_of<_impl::wrapper<Rep, Tag>> : std::type_identity<Rep> {
    };
}

#endif  // CNL_IMPL_WRAPPER_REP_OF_H
