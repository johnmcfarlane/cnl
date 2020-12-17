
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_REP_H)
#define CNL_IMPL_NUMBER_REP_H

#include "../type_traits/type_identity.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag>
    struct rep<_impl::number<Rep, Tag>> : _impl::type_identity<Rep> {
    };
}

#endif // CNL_IMPL_NUMBER_REP_H
