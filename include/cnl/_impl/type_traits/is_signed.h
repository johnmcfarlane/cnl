
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_IS_SIGNED
#define CNL_IMPL_TYPE_TRAITS_IS_SIGNED

#include "../../limits.h"

#include <type_traits>

namespace cnl {
    template<class T>
    struct is_signed : std::integral_constant<bool, numeric_limits<T>::is_signed> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_SIGNED
