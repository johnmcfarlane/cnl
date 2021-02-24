
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_IS_TAG_H)
#define CNL_IMPL_ROUNDING_IS_TAG_H

#include "../operators/tag.h"
#include "is_rounding_tag.h"

/// compositional numeric library
namespace cnl {
    template<rounding_tag T>
    inline constexpr auto is_tag<T> = true;
}

#endif  // CNL_IMPL_ROUNDING_IS_TAG_H
