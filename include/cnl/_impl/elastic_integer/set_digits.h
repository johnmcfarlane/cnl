
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_SET_DIGITS_H)
#define CNL_IMPL_ELASTIC_INTEGER_SET_DIGITS_H

#include "../num_traits/set_digits.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest, int MinNumBits>
    struct set_digits<elastic_integer<Digits, Narrowest>, MinNumBits> {
        using type = elastic_integer<MinNumBits, Narrowest>;
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_SET_DIGITS_H
