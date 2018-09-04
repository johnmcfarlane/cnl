
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_DIGITS_H)
#define CNL_IMPL_INTEGER_DIGITS_H

#include "type.h"
#include "../num_traits/digits.h"

/// compositional numeric library
namespace cnl {
    template<class Rep>
    struct digits<_impl::integer<Rep>>
            : digits<Rep> {
    };
}

#endif  // CNL_IMPL_INTEGER_DIGITS_H
