
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SET_TAG_H)
#define CNL_IMPL_NUMBER_SET_TAG_H

#include "../num_traits/set_tag.h"
#include "../type_traits/type_identity.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class InTag, class OutTag>
    struct set_tag<_impl::number<Rep, InTag>, OutTag> : _impl::type_identity<_impl::number<Rep, OutTag>> {
    };
}

#endif  // CNL_IMPL_NUMBER_SET_TAG_H
