
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_INTEGER_NUMERIC_LIMITS_H

#include "type.h"
#include "../../limits.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for overflow_integer

    template<class Rep>
    struct numeric_limits<_impl::integer<Rep>>
            : numeric_limits<_impl::number_base<_impl::integer<Rep>, Rep>> {
        static constexpr bool is_integer = true;
    };

    template<class Rep>
    struct numeric_limits<_impl::integer<Rep> const>
            : numeric_limits<_impl::number_base<_impl::integer<Rep>, Rep>> {
        static constexpr bool is_integer = true;
    };
}

#endif  // CNL_IMPL_INTEGER_NUMERIC_LIMITS_H
