
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_REMOVE_SIGNEDNESS_H)
#define CNL_IMPL_INTEGER_REMOVE_SIGNEDNESS_H

#include "type.h"
#include "../type_traits/remove_signedness.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag>
    struct remove_signedness<_impl::integer<Rep, Tag>>
            : _impl::type_identity<_impl::integer<remove_signedness_t<Rep>, Tag>> {
    };
}

#endif  // CNL_IMPL_INTEGER_REMOVE_SIGNEDNESS_H
