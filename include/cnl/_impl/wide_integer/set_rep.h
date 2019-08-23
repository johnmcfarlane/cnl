
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SET_REP_H)
#define CNL_IMPL_WIDE_INTEGER_SET_REP_H

#include "forward_declaration.h"
#include "../num_traits/adopt_signedness.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_rep.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest, typename Rep>
    struct set_rep<_impl::wide_integer<Digits, Narrowest>, Rep>
            : _impl::type_identity<_impl::wide_integer<
                    Digits,
                    _impl::adopt_signedness_t<Narrowest, Rep>>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_SET_REP_H
