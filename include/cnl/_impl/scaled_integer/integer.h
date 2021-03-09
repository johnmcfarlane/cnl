
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_INTEGER_H)
#define CNL_IMPL_SCALED_INTEGER_INTEGER_H

#include "../../integer.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<integer Rep, scaled_tag Scale>
    struct is_integer<scaled_integer<Rep, Scale>> : std::false_type {
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_INTEGER_H
