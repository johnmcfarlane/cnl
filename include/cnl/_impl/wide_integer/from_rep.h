
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_FROM_REP_H)
#define CNL_IMPL_WIDE_INTEGER_FROM_REP_H

#include "type.h"
#include "../num_traits/digits.h"
#include "../num_traits/from_rep.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest, typename Rep>
    struct from_rep<_impl::wide_integer<Digits, Narrowest>, Rep> {
        constexpr auto operator()(Rep const& rep) const
        -> _impl::wide_integer<
                Digits,
                cnl::_impl::set_signedness_t<Narrowest, cnl::is_signed<Rep>::value>>
        {
            return rep;
        }
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_FROM_REP_H
