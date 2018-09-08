
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SET_DIGITS_H)
#define CNL_IMPL_WIDE_INTEGER_SET_DIGITS_H

#include "type.h"
#include "../num_traits/set_digits.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    template<int FromDigits, class Rep, int ToDigits>
    struct set_digits<_impl::wide_integer<FromDigits, Rep>, ToDigits>
            : _impl::type_identity<_impl::wide_integer<ToDigits, Rep>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_SET_DIGITS_H
