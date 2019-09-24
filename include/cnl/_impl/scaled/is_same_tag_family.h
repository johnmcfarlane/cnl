
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_SCALED_IS_SAME_TAG_FAMILY_H
#define CNL_IMPL_SCALED_IS_SAME_TAG_FAMILY_H

#include "is_scaled_tag.h"
#include "../operators/is_same_tag_family.h"
#include "../type_traits/enable_if.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Tag1, class Tag2>
        struct is_same_tag_family<
                Tag1, Tag2, enable_if_t<is_scaled_tag<Tag1>::value&&is_scaled_tag<Tag2>::value>>
            : std::true_type {
        };
    }
}

#endif //CNL_IMPL_SCALED_IS_SAME_TAG_FAMILY_H
