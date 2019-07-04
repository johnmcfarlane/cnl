
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_NUMERIC_LIMITS_H)
#define CNL_IMPL_NUMBER_NUMERIC_LIMITS_H

#include "definition.h"
#include "../../limits.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for overflow_integer

    template<typename Rep, class Tag>
    struct numeric_limits<_impl::number<Rep, Tag>>
            : numeric_limits<Rep> {
    };

    template<typename Rep, class Tag>
    struct numeric_limits<_impl::number<Rep, Tag> const>
            : numeric_limits<Rep const> {
    };
}

#endif  // CNL_IMPL_NUMBER_NUMERIC_LIMITS_H
