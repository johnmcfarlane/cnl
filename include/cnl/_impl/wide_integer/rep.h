
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_REP_H)
#define CNL_IMPL_WIDE_INTEGER_REP_H

#include "../common.h"
#include "../duplex_integer/instantiate_duplex_integer.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/rep.h"
#include "../num_traits/set_digits.h"
#include "../type_traits/enable_if.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    struct rep<
            _impl::wide_integer<Digits, Narrowest>,
            _impl::enable_if_t<(_impl::max_digits<Narrowest>::value>=Digits)>>
        : set_digits<Narrowest, _impl::max(Digits, int{digits<Narrowest>::value})> {
    };

    template<int Digits, typename Narrowest>
    struct rep<
            _impl::wide_integer<Digits, Narrowest>,
            _impl::enable_if_t<(_impl::max_digits<Narrowest>::value<Digits)>>
        : _impl::instantiate_duplex_integer<Digits, Narrowest> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_REP_H
