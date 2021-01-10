
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_IS_SAME_TAG_FAMILY_H)
#define CNL_IMPL_WIDE_TAG_IS_SAME_TAG_FAMILY_H

#include "../operators/is_same_tag_family.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<wide_tag Tag1, wide_tag Tag2>
        struct is_same_tag_family<Tag1, Tag2>
            : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_WIDE_TAG_IS_SAME_TAG_FAMILY_H
