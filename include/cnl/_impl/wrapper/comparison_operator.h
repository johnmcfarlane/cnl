
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H)
#define CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H

#include "../../limits.h"
#include "../num_traits/from_value.h"
#include "../operators/custom_operator.h"
#include "../operators/overloads.h"
#include "definition.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP number<>
    template<_impl::comparison_op Operator, _impl::floating_point Lhs, _impl::wrapped Rhs>
    struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number<> OP higher
    template<_impl::comparison_op Operator, _impl::wrapped Lhs, _impl::floating_point Rhs>
    struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number<>
    template<_impl::comparison_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Rhs, Lhs>::value struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number<> OP lower
    template<_impl::comparison_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Lhs, Rhs>::value struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    template<_impl::comparison_op Operator, typename LhsRep, typename RhsRep, tag Tag>
    struct custom_operator<Operator, operand<_impl::wrapper<LhsRep, Tag>>, operand<_impl::wrapper<RhsRep, Tag>>> {
        [[nodiscard]] constexpr auto operator()(
                _impl::wrapper<LhsRep, Tag> const& lhs, _impl::wrapper<RhsRep, Tag> const& rhs) const
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_COMPARISON_OPERATOR_H
