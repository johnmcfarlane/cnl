
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TAG_OF_H)
#define CNL_IMPL_NUMBER_TAG_OF_H

#include "../num_traits/tag_of.h"
#include "../type_traits/type_identity.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, tag Tag>
    struct tag_of<_impl::number<Rep, Tag>> : _impl::type_identity<Tag> {
    };
}

#endif  // CNL_IMPL_NUMBER_TAG_OF_H
