
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_IS_TAG_H)
#define CNL_IMPL_ELASTIC_TAG_IS_TAG_H

#include "../num_traits/tag.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    inline constexpr auto is_tag<elastic_tag<Digits, Narrowest>> = true;
}

#endif  // CNL_IMPL_ELASTIC_TAG_IS_TAG_H
