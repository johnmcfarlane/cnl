
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H)
#define CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H

#include "../type_traits/enable_if.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Tag1, class Tag2, class Enabled=void>
        struct is_same_tag_family : std::false_type {
        };
    }
}

#endif  // CNL_IMPL_OPERATORS_IS_SAME_TAG_FAMILY_H
