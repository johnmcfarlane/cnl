
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_FIXED_POINT_H)
#define CNL_FIXED_POINT_H

#include "integer.h"

/// compositional numeric library
namespace cnl {
    template<typename T>
    struct is_fixed_point : is_integer<T> {
    };

    template<typename T>
    inline constexpr auto is_fixed_point_v = is_fixed_point<T>::value;

    template<typename T>
    concept fixed_point = is_fixed_point_v<T>;
}

#endif  // CNL_FIXED_POINT_H
