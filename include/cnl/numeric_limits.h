
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief CNL alternative to \verbatim<limits>\endverbatim with 128-bit support

#if !defined(CNL_NUMERIC_LIMITS_H)
#define CNL_NUMERIC_LIMITS_H

#include "_impl/config.h"
#include "_impl/cstdint/types.h"

#include <climits>
#include <limits>

namespace cnl {

    template<class T>
    struct numeric_limits : std::numeric_limits<T> {
    };

    template<class T>
    struct numeric_limits<T const> : numeric_limits<T> {
    };
}

#endif  // CNL_NUMERIC_LIMITS_H
