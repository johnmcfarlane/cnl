
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_NUMBER_H)
#define CNL_NUMBER_H

#include "fixed_point.h"

#include <concepts>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename T>
    inline constexpr auto is_number_v = is_fixed_point_v<T> || std::is_floating_point_v<T>;

    template<typename T>
    concept number = is_number_v<T>;
}

#endif  // CNL_NUMBER_H
