
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_OPERATORS_H)
#define CNL_IMPL_INTEGER_OPERATORS_H

#include "from_rep.h"
#include "from_value.h"
#include "make_integer.h"
#include "numeric_limits.h"
#include "type.h"
#include "../operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Operator, typename Rep>
        struct unary_operator<Operator, integer<Rep>> {
            constexpr auto operator()(integer<Rep> const& rhs) const
            -> decltype(from_rep<integer<decltype(Operator()(_impl::to_rep(rhs)))>>(Operator()(_impl::to_rep(rhs))))
            {
                return from_rep<integer<decltype(Operator()(_impl::to_rep(rhs)))>>(Operator()(_impl::to_rep(rhs)));
            }
        };

        template<class Operator, typename LhsRep, typename RhsRep>
        struct binary_operator<Operator, integer<LhsRep>, integer<RhsRep>> {
            constexpr auto operator()(integer<LhsRep> const& lhs, integer<RhsRep> const& rhs) const
            -> decltype(make_integer(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
            {
                return make_integer(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };

        template<class Operator, typename LhsRep, typename RhsRep>
        struct comparison_operator<Operator, integer<LhsRep>, integer<RhsRep>> {
            constexpr auto operator()(integer<LhsRep> const& lhs, integer<RhsRep> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };
    }
}

#endif  // CNL_IMPL_INTEGER_OPERATORS_H
