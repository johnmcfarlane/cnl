
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H)
#define CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H

#include "../custom_operator/tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Tag1, typename Tag2>
        struct is_same_tag_family {
            static constexpr auto value =
                    std::is_same_v<Tag1, Tag2> && is_tag<Tag1> && is_tag<Tag2>;
        };
    }
}

#endif  // CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H
