
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ARITHMETIC_H)
#define CNL_ARITHMETIC_H

#include "_impl/custom_operator/tag.h"
#include "number.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename T>
    inline constexpr auto is_arithmetic_v = is_number_v<T> || is_tag<T>;

    template<typename T>
    concept arithmetic = is_arithmetic_v<T>;
}

#endif  // CNL_ARITHMETIC_H
