
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_CMATH_ABS
#define CNL_IMPL_CMATH_ABS

#include "../type_traits/is_signed.h"
#include "../type_traits/enable_if.h"

namespace cnl {
    namespace _impl {
        template<typename T>
        constexpr auto abs(T const& value)
        -> enable_if_t<is_signed<T>::value, T>
        {
            static_assert(
                    std::is_same<decltype(+value), decltype(-value)>::value,
                    "cnl::abs only supports types with symetrically-typed unary operators");

            return static_cast<T>((value<0) ? -value : +value);
        }

        template<typename T>
        constexpr auto abs(T const& value)
        -> enable_if_t<!is_signed<T>::value, T>
        {
            return value;
        }
    }
}

#endif  // CNL_IMPL_CMATH_ABS
