
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief \ref cnl::elastic_integer specializations of num_traits traits and similar

#if !defined(CNL_IMPL_ELASTIC_INTEGER_FROM_REP_H)
#define CNL_IMPL_ELASTIC_INTEGER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "../num_traits/set_rep.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest, class Rep>
    struct from_rep<elastic_integer<Digits, Narrowest>, Rep> {
        CNL_NODISCARD constexpr auto operator()(Rep const& r) const
        -> _impl::set_rep_t<elastic_integer<Digits, Narrowest>, Rep>
        {
            return r;
        }
    };

    template<typename ArchetypeRep, int Digits, typename Narrowest, typename Rep>
    struct from_rep<_impl::number<ArchetypeRep, elastic_tag<Digits, Narrowest>>, Rep>
            : from_rep<elastic_integer<Digits, Narrowest>, Rep> {
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_FROM_REP_H
