
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SET_DIGITS_H)
#define CNL_IMPL_NUMBER_SET_DIGITS_H

#include "../num_traits/set_digits.h"
#include "../type_traits/type_identity.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag, int Digits>
    struct set_digits<_impl::number<Rep, Tag>, Digits>
            : _impl::type_identity<_impl::number<set_digits_t<Rep, Digits>, Tag>> {
    };
}

#endif  // CNL_IMPL_NUMBER_SET_DIGITS_H
