
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_DIGITS_H)
#define CNL_IMPL_NUMBER_DIGITS_H

#include "../num_traits/digits.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag>
    struct digits<_impl::number<Rep, Tag>>
            : digits<Rep> {
    };
}

#endif  // CNL_IMPL_NUMBER_DIGITS_H
