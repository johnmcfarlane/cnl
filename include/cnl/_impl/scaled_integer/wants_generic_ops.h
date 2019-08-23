
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_WANTS_GENERIC_OPS_H)
#define CNL_IMPL_SCALED_INTEGER_WANTS_GENERIC_OPS_H 1

#include "declaration.h"
#include "../operators/generic.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep, class Scale>
        struct wants_generic_ops<scaled_integer<Rep, Scale>> : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_WANTS_GENERIC_OPS_H
