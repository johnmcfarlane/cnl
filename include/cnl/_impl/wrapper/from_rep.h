
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_FROM_REP_H)
#define CNL_IMPL_WRAPPER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "../num_traits/tag.h"
#include "definition.h"
#include "set_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename NumberRep, tag NumberTag, typename Rep>
    struct from_rep<_impl::wrapper<NumberRep, NumberTag>, Rep> {
        [[nodiscard]] constexpr auto operator()(Rep const& rep) const
                -> _impl::set_rep_t<_impl::wrapper<NumberRep, NumberTag>, Rep>
        {
            return rep;
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_FROM_REP_H
