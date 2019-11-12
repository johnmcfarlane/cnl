
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_SET_DIGITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_SET_DIGITS_H

#include "../num_traits/set_digits.h"
#include "forward_declaration.h"
#include "instantiate_duplex_integer.h"
#include "rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower, int Digits>
    struct set_digits<_impl::duplex_integer<Upper, Lower>, Digits>
            : _impl::instantiate_duplex_integer<Digits, _impl::rep_t<_impl::duplex_integer<Upper, Lower>>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_SET_DIGITS_H
