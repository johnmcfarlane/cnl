
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_SET_ROUNDING_H)
#define CNL_IMPL_SCALED_INTEGER_SET_ROUNDING_H 1

#include "declaration.h"
#include "../num_traits/set_rounding.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Scale, class RoundingTag>
    struct set_rounding<scaled_integer<Rep, Scale>, RoundingTag>
            : _impl::type_identity<scaled_integer<set_rounding_t<Rep, RoundingTag>, Scale>> {
    };
}

#endif // CNL_IMPL_SCALED_INTEGER_SET_ROUNDING_H
