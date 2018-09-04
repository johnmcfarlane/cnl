
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_MAKE_INTEGER_H)
#define CNL_IMPL_INTEGER_MAKE_INTEGER_H

#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep>
        constexpr auto make_integer(Rep const& rep)
        -> integer<Rep> {
            return rep;
        }
    }
}

#endif  // CNL_IMPL_INTEGER_MAKE_INTEGER_H
