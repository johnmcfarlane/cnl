
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_BINARY_OPERATOR_H)
#define CNL_IMPL_WRAPPER_BINARY_OPERATOR_H

#include "../../limits.h"
#include "../num_traits/set_rep.h"
#include "../num_traits/set_tag.h"
#include "../operators/custom_operator.h"
#include "../operators/is_same_tag_family.h"
#include "../operators/native_tag.h"
#include "../operators/operators.h"
#include "../operators/overloads.h"
#include "is_wrapper.h"
#include "make_wrapper.h"
#include "numbers.h"
#include "operator_helpers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // higher OP number<>
    template<_impl::binary_arithmetic_op Operator, _impl::floating_point Lhs, _impl::wrapped Rhs>
    struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, static_cast<Lhs>(rhs));
        }
    };

    // number<> OP higher
    template<_impl::binary_arithmetic_op Operator, _impl::wrapped Lhs, _impl::floating_point Rhs>
    struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(static_cast<Rhs>(lhs), rhs);
        }
    };

    // lower OP number<>
    template<_impl::binary_arithmetic_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Rhs, Lhs>::value struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(_impl::from_value<Rhs>(lhs), rhs);
        }
    };

    // number<> OP lower
    template<_impl::binary_arithmetic_op Operator, class Lhs, class Rhs>
    requires _impl::number_can_wrap<Lhs, Rhs>::value struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, _impl::from_value<Lhs>(rhs));
        }
    };

    template<_impl::binary_arithmetic_op Operator, _impl::wrapped Lhs, _impl::wrapped Rhs>
    requires(_impl::is_same_tag_family<_impl::tag_of_t<Lhs>, _impl::tag_of_t<Rhs>>::value) struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        using _rep_operator = custom_operator<
                Operator,
                operand<_impl::rep_of_t<Lhs>, _impl::tag_of_t<Lhs>>,
                operand<_impl::rep_of_t<Rhs>, _impl::tag_of_t<Rhs>>>;
        using _result_rep = decltype(_rep_operator{}(
                _impl::to_rep(std::declval<Lhs>()), _impl::to_rep(std::declval<Rhs>())));
        using _result_tag = _impl::op_result<Operator, _impl::tag_of_t<Lhs>, _impl::tag_of_t<Rhs>>;
        using _result_archetype = _impl::set_rep_t<_impl::set_tag_t<Lhs, _result_tag>, _result_rep>;

        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<_result_archetype>(
                    _rep_operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_BINARY_OPERATOR_H
