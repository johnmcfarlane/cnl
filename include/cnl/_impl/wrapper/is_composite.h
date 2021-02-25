
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_IS_COMPOSITE_H)
#define CNL_IMPL_WRAPPER_IS_COMPOSITE_H

#include "../num_traits/is_composite.h"
#include "is_wrapper.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<_impl::any_wrapper Number>
    struct is_composite<Number> : std::true_type {
    };
}

#endif  // CNL_IMPL_WRAPPER_IS_COMPOSITE_H
