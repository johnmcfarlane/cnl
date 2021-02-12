
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_ABS_H)
#define CNL_IMPL_ELASTIC_INTEGER_ABS_H

#include "../cmath/abs.h"
#include "definition.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits, typename Narrowest>
        [[nodiscard]] constexpr auto abs(elastic_integer<Digits, Narrowest> const& n)
        {
            return from_rep<elastic_integer<Digits, Narrowest>>(abs(to_rep(n)));
        }
    }
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_ABS_H
