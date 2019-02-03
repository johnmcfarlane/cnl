
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_REP_H)
#define CNL_IMPL_WIDE_INTEGER_REP_H

#include "forward_declaration.h"
#include "../duplex_integer/instantiate_duplex_integer.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../type_traits/enable_if.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::wide_integer_rep
        template<int Digits, typename Narrowest, typename Enable = void>
        struct wide_integer_rep;

        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value>=Digits)>>
                : set_digits<Narrowest, max(Digits, digits<Narrowest>::value)> {
        };

        template<int Digits, typename Narrowest>
        struct wide_integer_rep<Digits, Narrowest, enable_if_t<(max_digits<Narrowest>::value<Digits)>>
                : instantiate_duplex_integer<Digits, Narrowest> {
        };

        template<int Digits, typename Narrowest>
        using wide_integer_rep_t = typename wide_integer_rep<Digits, Narrowest>::type;
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_REP_H
