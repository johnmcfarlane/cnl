
//          Copyright Heikki Berg 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H

#include "../cmath/abs.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/native_tag.h"
#include "is_rounding_tag.h"
#include "is_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify tie to positive inf rounding behavior in arithmetic operations
    ///
    /// Arithmetic operations using this tag round round the mid-point value to positive infinity
    /// direction and round all other values to their nearest representable value.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply,
    /// cnl::tie_to_pos_inf_rounding_tag
    struct tie_to_pos_inf_rounding_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<tie_to_pos_inf_rounding_tag> : std::true_type {
        };
    }

    template<_impl::unary_arithmetic_op Operator, typename Operand>
    struct custom_operator<Operator, op_value<Operand, tie_to_pos_inf_rounding_tag>>
        : custom_operator<Operator, op_value<Operand, _impl::native_tag>> {
    };

    template<_impl::binary_arithmetic_op Operator, typename Lhs, typename Rhs>
    struct custom_operator<
            Operator,
            op_value<Lhs, tie_to_pos_inf_rounding_tag>,
            op_value<Rhs, tie_to_pos_inf_rounding_tag>>
        : Operator {
    };

    template<typename Lhs, typename Rhs>
    struct custom_operator<
            _impl::divide_op,
            op_value<Lhs, tie_to_pos_inf_rounding_tag>,
            op_value<Rhs, tie_to_pos_inf_rounding_tag>> {
    private:
        using result_type = decltype(std::declval<Lhs>() / std::declval<Rhs>());

        template<typename LhsParam, typename RhsParam>
        [[nodiscard]] constexpr auto step2(LhsParam const& lhs, RhsParam const& rhs) const
                -> result_type
        {
            return (lhs < 0) ? -((_impl::abs(lhs) + (rhs - (lhs < 0 ? 1 : 0)) / 2) / rhs)
                             : +((_impl::abs(lhs) + (rhs - (lhs < 0 ? 1 : 0)) / 2) / rhs);
        }
        [[nodiscard]] constexpr auto step1(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            return (rhs < 0) ? step2(-lhs, -rhs) : step2(lhs, rhs);
        }

    public:
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            return step1(lhs, rhs);
        }
    };

    template<_impl::shift_op Operator, typename Lhs, typename Rhs, tag RhsTag>
    struct custom_operator<Operator, op_value<Lhs, tie_to_pos_inf_rounding_tag>, op_value<Rhs, RhsTag>> : Operator {
    };

    template<_impl::prefix_op Operator, typename Rhs>
    struct custom_operator<Operator, op_value<Rhs, tie_to_pos_inf_rounding_tag>> : Operator {
    };

    template<_impl::postfix_op Operator, typename Lhs>
    struct custom_operator<Operator, op_value<Lhs, tie_to_pos_inf_rounding_tag>> : Operator {
    };
}

#endif  // CNL_IMPL_ROUNDING_TIE_TO_POS_INF_ROUNDING_TAG_H
