
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief file containing definitions that might go in \verbatim<cmath>\endverbatim if they were standard

#if !defined(CNL_CMATH_H)
#define CNL_CMATH_H

#include "_impl/cmath/abs.h"

#include <cmath>

/// compositional numeric library
namespace cnl {
    using _impl::abs;

    using std::sqrt;
}

#endif //CNL_CMATH_H
