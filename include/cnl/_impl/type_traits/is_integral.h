
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL_H)
#define CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL_H

#include "../config.h"
#include "../cstdint/types.h"

#include <type_traits>

namespace cnl {
    namespace _impl {
        template<class T>
        struct is_integral : std::is_integral<T> {
        };

#if defined(CNL_INT128_ENABLED)
        template<>
        struct is_integral<int128> : std::true_type {
        };

        template<>
        struct is_integral<uint128> : std::true_type {
        };
#endif

        template<class T>
        inline constexpr auto is_integral_v = is_integral<T>::value;

        template<typename T>
        concept integral = is_integral_v<T>;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL_H
