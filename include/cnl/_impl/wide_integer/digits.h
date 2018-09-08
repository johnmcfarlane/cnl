
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_DIGITS_H)
#define CNL_IMPL_WIDE_INTEGER_DIGITS_H

#include "type.h"
#include "../num_traits/digits.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    struct digits<_impl::wide_integer<Digits, Narrowest>>
            : std::integral_constant<int, Digits> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_DIGITS_H
