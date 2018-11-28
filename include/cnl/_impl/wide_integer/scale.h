
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SCALE_H)
#define CNL_IMPL_WIDE_INTEGER_SCALE_H

#include "../num_traits/scale.h"
#include "operators.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, int Radix, int WideIntegerDigits, typename WideIntegerNarrowest>
    struct scale<Digits, Radix, _impl::wide_integer<WideIntegerDigits, WideIntegerNarrowest>>
            : _impl::default_scale<Digits, Radix, _impl::wide_integer<WideIntegerDigits, WideIntegerNarrowest>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_SCALE_H
