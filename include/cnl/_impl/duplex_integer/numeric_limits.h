
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H

#include "../../limits.h"
#include "../num_traits/from_rep.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Lower, typename Upper>
    struct numeric_limits<_impl::duplex_integer<Lower, Upper>>
            : numeric_limits<Lower> {
        static constexpr bool is_integer = true;
        using _lower_numeric_limits = numeric_limits<Lower>;
        using _upper_numeric_limits = numeric_limits<Upper>;
        using _value_type = _impl::duplex_integer<Lower, Upper>;

        // standard members
        static constexpr int digits = _lower_numeric_limits::digits*_upper_numeric_limits::digits;

        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H
