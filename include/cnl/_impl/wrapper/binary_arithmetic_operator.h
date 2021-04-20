
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_BINARY_OPERATOR_H)
#define CNL_IMPL_WRAPPER_BINARY_OPERATOR_H

#include "../../floating_point.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/is_same_tag_family.h"
#include "../custom_operator/native_tag.h"
#include "../custom_operator/op.h"
#include "../custom_operator/overloads.h"
#include "../num_traits/set_rep.h"
#include "../num_traits/set_tag.h"
#include "is_wrapper.h"
#include "make_wrapper.h"
#include "numbers.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP any_wrapper
    template<_impl::binary_arithmetic_op Operator, floating_point Lhs, _impl::any_wrapper Rhs>
    struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // any_wrapper OP higher
    template<_impl::binary_arithmetic_op Operator, _impl::any_wrapper Lhs, floating_point Rhs>
    struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP any_wrapper
    template<_impl::binary_arithmetic_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Rhs, Lhs>::value struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // any_wrapper OP lower
    template<_impl::binary_arithmetic_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Lhs, Rhs>::value struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, from_value<Lhs, Rhs>{}(rhs));
        }
    };

    template<_impl::binary_arithmetic_op Operator, _impl::any_wrapper Lhs, _impl::any_wrapper Rhs>
    requires(_impl::is_same_tag_family<_impl::tag_of_t<Lhs>, _impl::tag_of_t<Rhs>>::value) struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            auto const lhs_rep{_impl::to_rep(lhs)};
            auto const rhs_rep{_impl::to_rep(rhs)};
            using rep_operator = custom_operator<
                    Operator,
                    op_value<std::remove_const_t<decltype(lhs_rep)>, _impl::tag_of_t<Lhs>>,
                    op_value<std::remove_const_t<decltype(rhs_rep)>, _impl::tag_of_t<Rhs>>>;
            auto const result_rep{rep_operator{}(lhs_rep, rhs_rep)};
            using result_rep_type = std::remove_const_t<decltype(result_rep)>;
            using result_tag = _impl::op_result<Operator, _impl::tag_of_t<Lhs>, _impl::tag_of_t<Rhs>>;
            using result_archetype = _impl::set_rep_t<_impl::set_tag_t<Lhs, result_tag>, result_rep_type>;

            return _impl::from_rep<result_archetype>(result_rep);
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_BINARY_OPERATOR_H
