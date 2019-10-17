
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_SET_REP_H)
#define CNL_IMPL_ELASTIC_INTEGER_SET_REP_H

#include "../num_traits/adopt_width.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_rep.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref elastic_integer specialization of \ref to_rep
    /// \headerfile cnl/elastic_integer.h
    template<int Digits, typename Narrowest, typename Rep>
    struct set_rep<elastic_integer<Digits, Narrowest>, Rep>
            : _impl::type_identity<elastic_integer<Digits, _impl::adopt_width_t<Rep, Narrowest>>> {
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_SET_REP_H
