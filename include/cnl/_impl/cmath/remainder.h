
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_CMATH_REMAINDER_H
#define CNL_IMPL_CMATH_REMAINDER_H

#include "../../limits.h"
#include "../type_traits/enable_if.h"
#include "abs.h"

#include <cmath>

namespace cnl {
    namespace _impl {
#if defined(__clang__) || defined(_MSC_VER)
        template<typename T>
        constexpr auto remainder(T const& n, T const& d)
        -> enable_if_t<numeric_limits<T>::is_integer, decltype(abs(n % d))> {
            return abs(n % d);
        }
#else
        using std::remainder;
#endif
    }
}

#endif  // CNL_IMPL_CMATH_REMAINDER_H
