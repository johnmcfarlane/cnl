
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_IS_TAG_H)
#define CNL_IMPL_SCALED_IS_TAG_H

#include "../custom_operator/tag.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Exponent, int Radix>
    inline constexpr auto is_tag<power<Exponent, Radix>> = true;
}

#endif  // CNL_IMPL_SCALED_IS_TAG_H
