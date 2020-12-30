
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_SET_REP_H)
#define CNL_IMPL_SCALED_INTEGER_SET_REP_H

#include "../num_traits/set_rep.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename InRep, class Scale, typename OutRep>
    struct set_rep<scaled_integer<InRep, Scale>, OutRep>
        : std::type_identity<scaled_integer<OutRep, Scale>> {
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_SET_REP_H
