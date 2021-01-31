
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_WIDTH)
#define CNL_IMPL_TYPE_TRAITS_WIDTH

#include "../numbers/signedness.h"
#include "digits.h"

namespace cnl {
    namespace _impl {
        template<typename T>
        inline constexpr int width = digits<T> + numbers::signedness_v<T>;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_WIDTH
