
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_FROM_VALUE_H)
#define CNL_IMPL_INTEGER_FROM_VALUE_H

#include "type.h"
#include "../num_traits/from_value.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, typename Value>
    struct from_value<_impl::integer<Rep>, Value>
            : _impl::from_value_simple<_impl::integer<Value>, Value> {
    };
}

#endif  // CNL_IMPL_INTEGER_FROM_VALUE_H
