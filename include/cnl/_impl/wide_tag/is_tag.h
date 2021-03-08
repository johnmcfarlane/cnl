
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_IS_TAG_H)
#define CNL_IMPL_WIDE_TAG_IS_TAG_H

#include "../custom_operator/tag.h"
#include "declaration.h"
#include "is_wide_tag.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    inline constexpr auto is_tag<wide_tag<Digits, Narrowest>> = true;
}

#endif  // CNL_IMPL_WIDE_TAG_IS_TAG_H
