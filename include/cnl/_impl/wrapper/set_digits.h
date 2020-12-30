
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_SET_DIGITS_H)
#define CNL_IMPL_WRAPPER_SET_DIGITS_H

#include "../num_traits/set_digits.h"
#include "../num_traits/tag.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Rep, tag Tag, int Digits>
    struct set_digits<_impl::wrapper<Rep, Tag>, Digits>
        : std::type_identity<_impl::wrapper<set_digits_t<Rep, Digits>, Tag>> {
    };
}

#endif  // CNL_IMPL_WRAPPER_SET_DIGITS_H
