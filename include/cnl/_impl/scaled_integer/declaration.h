
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_DECLARATION_H)
#define CNL_IMPL_SCALED_INTEGER_DECLARATION_H 1

#include "../../limits.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep = int, int Exponent = 0, int Radix = cnl::numeric_limits<Rep>::radix>
    class scaled_integer;
}

#endif  // CNL_IMPL_SCALED_INTEGER_DECLARATION_H