
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_FLOATING_POINT_H)
#define CNL_FLOATING_POINT_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename T>
    struct is_floating_point : std::is_floating_point<T> {
    };

    template<typename T>
    inline constexpr auto is_floating_point_v = is_floating_point<T>::value;

    template<typename T>
    concept floating_point = is_floating_point_v<T>;
}

#endif  // CNL_FLOATING_POINT_H
