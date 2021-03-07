
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_INTEGER_H)
#define CNL_INTEGER_H

#include "_impl/type_traits/is_integral.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename T>
    struct is_integer : _impl::is_integral<T> {
    };

    template<typename T>
    inline constexpr auto is_integer_v = is_integer<T>::value;

    template<typename T>
    concept integer = is_integer_v<T>;
}

#endif  // CNL_INTEGER_H
