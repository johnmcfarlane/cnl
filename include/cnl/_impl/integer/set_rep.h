
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_SET_REP_H)
#define CNL_IMPL_INTEGER_SET_REP_H

#include "../num_traits/set_rep.h"
#include "../type_traits/type_identity.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename IntegerRep, typename Rep>
    struct set_rep<_impl::integer<IntegerRep>, Rep>
            : _impl::type_identity<_impl::integer<Rep>> {
    };
}

#endif  // CNL_IMPL_INTEGER_SET_REP_H
