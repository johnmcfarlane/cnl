
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL)
#define CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL

#include "../../cstdint.h"

#include "../config.h"

#include <type_traits>

namespace cnl {
    namespace _impl {
        template<class T>
        struct is_integral : std::is_integral<T> {
        };

#if defined(CNL_INT128_ENABLED)
        template<>
        struct is_integral<int128> : std::integral_constant<bool, true> {
        };

        template<>
        struct is_integral<uint128> : std::integral_constant<bool, true> {
        };
#endif

        template<class T>
        constexpr int is_integral_v = is_integral<T>::value;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL
