
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CMATH_ABS)
#define CNL_IMPL_CMATH_ABS

#include "../numbers/signedness.h"

namespace cnl {
    namespace _impl {
        template<typename T>
        requires(numbers::signedness_v<T>)
                CNL_NODISCARD constexpr auto abs(T const& value)
        {
            static_assert(std::is_same<decltype(+value), decltype(-value)>::value);

            return static_cast<T>((value < 0) ? -value : +value);
        }

        template<typename T>
        requires(!numbers::signedness_v<T>)
                CNL_NODISCARD constexpr auto abs(T const& value)
        {
            return value;
        }
    }
}

#endif  // CNL_IMPL_CMATH_ABS
