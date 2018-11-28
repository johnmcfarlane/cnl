
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_FORWARD_DECLARATION_H)
#define CNL_IMPL_WIDE_INTEGER_FORWARD_DECLARATION_H

#include "../num_traits/digits.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits = digits<int>::value, typename Narrowest = int>
        class wide_integer;
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_FORWARD_DECLARATION_H
