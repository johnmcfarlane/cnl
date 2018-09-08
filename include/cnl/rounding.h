
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the integer rounding

#if !defined(CNL_ROUNDING_H)
#define CNL_ROUNDING_H

#include "_impl/rounding.h"

/// compositional numeric library
namespace cnl {
    using _impl::nearest_rounding_tag;
    using _impl::native_rounding_tag;

    using _impl::divide;
    using _impl::shift_right;
}

#endif  // CNL_ROUNDING_H
