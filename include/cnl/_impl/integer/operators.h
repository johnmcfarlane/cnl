
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_OPERATORS_H)
#define CNL_IMPL_INTEGER_OPERATORS_H

#include "../operators/generic.h"
#include "from_rep.h"
#include "from_value.h"
#include "make_integer.h"
#include "numeric_limits.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Operator, typename Rep>
    struct unary_operator<_impl::native_tag, Operator, _impl::integer<Rep>> {
        CNL_NODISCARD constexpr auto operator()(_impl::integer<Rep> const& rhs) const
        -> decltype(_impl::from_rep<_impl::integer<decltype(Operator()(_impl::to_rep(rhs)))>>(
                Operator()(_impl::to_rep(rhs))))
        {
            return _impl::from_rep<_impl::integer<decltype(Operator()(_impl::to_rep(rhs)))>>(
                    Operator()(_impl::to_rep(rhs)));
        }
    };

    template<class Operator, typename LhsRep, typename RhsRep>
    struct binary_operator<_impl::native_tag, Operator, _impl::integer<LhsRep>, _impl::integer<RhsRep>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::integer<LhsRep> const& lhs, _impl::integer<RhsRep> const& rhs) const
        -> decltype(_impl::make_integer(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
        {
            return _impl::make_integer(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };

    template<class Operator, typename LhsRep, typename RhsRep>
    struct comparison_operator<Operator, _impl::integer<LhsRep>, _impl::integer<RhsRep>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::integer<LhsRep> const& lhs, _impl::integer<RhsRep> const& rhs) const
        -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };
}

#endif  // CNL_IMPL_INTEGER_OPERATORS_H
