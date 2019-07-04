
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_IS_COMPOSITE_H)
#define CNL_IMPL_INTEGER_IS_COMPOSITE_H

#include "declaration.h"
#include "../num_traits/is_composite.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag>
    struct is_composite<_impl::integer<Rep, Tag>>
    : std::true_type {
};
}

#endif  // CNL_IMPL_INTEGER_IS_COMPOSITE_H
