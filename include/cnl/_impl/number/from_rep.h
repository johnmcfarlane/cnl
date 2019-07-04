
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_FROM_REP_H)
#define CNL_IMPL_NUMBER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename NumberRep, typename Rep>
    struct from_rep<_impl::number<NumberRep>, Rep> {
        CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
        -> _impl::number<Rep>
        {
            return rep;
        }
    };
}

#endif  // CNL_IMPL_NUMBER_FROM_REP_H
