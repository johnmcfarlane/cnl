
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_WIDTH)
#define CNL_IMPL_TYPE_TRAITS_WIDTH

#include "../type_traits/is_signed.h"
#include "digits.h"
#include <type_traits>

namespace cnl {
    namespace _impl {
        template<typename T>
        struct width : std::integral_constant<int, digits<T>::value+is_signed<T>::value> {
        };
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_WIDTH
