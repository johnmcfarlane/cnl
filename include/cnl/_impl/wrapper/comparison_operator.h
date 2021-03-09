
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H)
#define CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H

#include "../../floating_point.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/overloads.h"
#include "../num_traits/from_value.h"
#include "definition.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP wrapper
    template<_impl::comparison_op Operator, floating_point Lhs, _impl::any_wrapper Rhs>
    struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // wrapper OP higher
    template<_impl::comparison_op Operator, _impl::any_wrapper Lhs, floating_point Rhs>
    struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP wrapper
    template<_impl::comparison_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Rhs, Lhs>::value struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // wrapper OP lower
    template<_impl::comparison_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Lhs, Rhs>::value struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, from_value<Lhs, Rhs>{}(rhs));
        }
    };

    template<_impl::comparison_op Operator, typename LhsRep, typename RhsRep, tag Tag>
    struct custom_operator<Operator, op_value<_impl::wrapper<LhsRep, Tag>>, op_value<_impl::wrapper<RhsRep, Tag>>> {
        [[nodiscard]] constexpr auto operator()(
                _impl::wrapper<LhsRep, Tag> const& lhs, _impl::wrapper<RhsRep, Tag> const& rhs) const
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H
