
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_SET_DIGITS_H)
#define CNL_IMPL_INTEGER_SET_DIGITS_H

#include "type.h"
#include "../num_traits/set_digits.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    template<class Rep, int Digits>
    struct set_digits<_impl::integer<Rep>, Digits>
            : _impl::type_identity<_impl::integer<set_digits_t<Rep, Digits>>> {
    };
}

#endif  // CNL_IMPL_INTEGER_SET_DIGITS_H
