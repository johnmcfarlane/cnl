
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL)
#define CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL

#include "../config.h"
#include "../cstdint/types.h"

#include <type_traits>

namespace cnl {
    namespace _impl {
        template<class T>
        inline constexpr auto is_integral = std::is_integral_v<T>;

#if defined(CNL_INT128_ENABLED)
        template<>
        inline constexpr auto is_integral<int128> = true;

        template<>
        inline constexpr auto is_integral<uint128> = true;
#endif

        template<typename T>
        concept integral = is_integral<T>;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_INTEGRAL
