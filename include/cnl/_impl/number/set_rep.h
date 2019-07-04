
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SET_REP_H)
#define CNL_IMPL_NUMBER_SET_REP_H

#include "declaration.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    template<typename InRep, class Tag, typename OutRep>
    struct set_rep<_impl::number<InRep, Tag>, OutRep> : _impl::type_identity<_impl::number<OutRep, Tag>> {
    };
}

#endif  // CNL_IMPL_NUMBER_SET_REP_H
