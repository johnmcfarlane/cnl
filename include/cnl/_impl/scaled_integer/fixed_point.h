
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_FIXED_POINT_H)
#define CNL_IMPL_SCALED_INTEGER_FIXED_POINT_H

#include "../../fixed_point.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Scale>
    struct is_fixed_point<scaled_integer<Rep, Scale>> : std::true_type {
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_FIXED_POINT_H
