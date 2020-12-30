
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_DIGITS_H)
#define CNL_IMPL_ELASTIC_INTEGER_DIGITS_H

#include "../num_traits/digits.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest>
    inline constexpr auto digits<elastic_integer<Digits, Narrowest>> = Digits;
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_DIGITS_H
