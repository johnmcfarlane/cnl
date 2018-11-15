
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_FROM_REP_H)
#define CNL_IMPL_MULTIWORD_INTEGER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Word, int NumWords, typename Rep>
    struct from_rep<_impl::multiword_integer<Word, NumWords>, Rep> {
        constexpr auto operator()(Rep const& rep) const
        -> _impl::multiword_integer<Word, NumWords>
        {
            return rep;
        }
    };
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_FROM_REP_H
