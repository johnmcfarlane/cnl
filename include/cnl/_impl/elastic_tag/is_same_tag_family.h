
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_IS_SAME_TAG_FAMILY_H)
#define CNL_IMPL_ELASTIC_TAG_IS_SAME_TAG_FAMILY_H

#include "../custom_operator/is_same_tag_family.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits1, typename Narrowest1, int Digits2, typename Narrowest2>
        struct is_same_tag_family<
                elastic_tag<Digits1, Narrowest1>, elastic_tag<Digits2, Narrowest2>>
            : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_ELASTIC_TAG_IS_SAME_TAG_FAMILY_H
