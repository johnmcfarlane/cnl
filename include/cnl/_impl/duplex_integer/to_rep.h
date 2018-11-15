
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_TO_REP_H)
#define CNL_IMPL_DUPLEX_INTEGER_TO_REP_H 1

#include "forward_declaration.h"
#include "../num_traits/to_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct to_rep<_impl::duplex_integer<Upper, Lower>> {
        constexpr auto operator()(_impl::duplex_integer<Upper, Lower> const& number) const
        -> _impl::to_rep_t<Upper>
        {
            return _impl::to_rep(Upper(number));
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_TO_REP_H
