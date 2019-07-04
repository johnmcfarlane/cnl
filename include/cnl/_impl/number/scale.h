
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SCALE_H)
#define CNL_IMPL_NUMBER_SCALE_H

#include "../num_traits/scale.h"
#include "definition.h"
#include "digits.h"
#include "operators.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, int Radix, class Rep>
    struct scale<Digits, Radix, _impl::number<Rep>>
            : _impl::default_scale<Digits, Radix, _impl::number<Rep>> {
    };
}

#endif  // CNL_IMPL_NUMBER_SCALE_H
