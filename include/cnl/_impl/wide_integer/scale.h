
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SCALE_H)
#define CNL_IMPL_WIDE_INTEGER_SCALE_H

#include "../num_traits/scale.h"
#include "definition.h"
#include "generic.h"

/// compositional numeric library
namespace cnl {
    template<
            int Digits, int Radix, typename NumberRep, int NumberTagDigits,
            typename NumberTagNarrowest>
    struct scale<
            Digits, Radix, _impl::number<NumberRep, wide_tag<NumberTagDigits, NumberTagNarrowest>>>
        : _impl::default_scale<
                  Digits, Radix,
                  _impl::number<NumberRep, wide_tag<NumberTagDigits, NumberTagNarrowest>>> {
    };
}

#endif // CNL_IMPL_WIDE_INTEGER_SCALE_H
