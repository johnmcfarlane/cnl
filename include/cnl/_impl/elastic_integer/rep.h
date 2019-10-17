
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_REP_H)
#define CNL_IMPL_ELASTIC_INTEGER_REP_H

#include "../num_traits/digits.h"
#include "../num_traits/rep.h"
#include "../num_traits/set_digits.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref elastic_integer specialization of \ref to_rep
    /// \headerfile cnl/elastic_integer.h
    template<int Digits, typename Narrowest>
    struct rep<elastic_integer<Digits, Narrowest>>
            : _impl::type_identity<typename elastic_tag<Digits, Narrowest>::_rep> {
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_REP_H
