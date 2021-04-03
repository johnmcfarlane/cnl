
//          Copyright Heikki Berg 2020
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H

#include "../cmath/abs.h"
#include "../custom_operator/definition.h"
#include "../custom_operator/native_tag.h"
#include "is_rounding_tag.h"
#include "is_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify floor or round towards minus infinity rounding behavior in arithmetic
    /// operations
    ///
    /// Arithmetic operations using this tag round by truncating 2's complement binary
    /// representation, thus round all values towards minus infinity.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer,
    /// cnl::convert, cnl::left_shift,
    /// cnl::neg_inf_rounding_tag
    struct neg_inf_rounding_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<neg_inf_rounding_tag> : std::true_type {
        };
    }

    template<_impl::unary_arithmetic_op Operator, typename Operand>
    struct custom_operator<Operator, op_value<Operand, neg_inf_rounding_tag>>
        : custom_operator<Operator, op_value<Operand, _impl::native_tag>> {
    };

    template<_impl::binary_arithmetic_op Operator, typename Lhs, typename Rhs>
    struct custom_operator<Operator, op_value<Lhs, neg_inf_rounding_tag>, op_value<Rhs, neg_inf_rounding_tag>>
        : Operator {
    };

    template<typename Lhs, typename Rhs>
    struct custom_operator<_impl::divide_op, op_value<Lhs, neg_inf_rounding_tag>, op_value<Rhs, neg_inf_rounding_tag>> {
    private:
        using result_type = decltype(std::declval<Lhs>() / std::declval<Rhs>());
        [[nodiscard]] constexpr auto remainder(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            return lhs % rhs;
        }
        [[nodiscard]] constexpr auto division(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            return lhs / rhs;
        }
        [[nodiscard]] constexpr auto division_neg(Lhs const& lhs, Rhs const& rhs) const
                -> result_type
        {
            return lhs / rhs - 1;
        }

    public:
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type
        {
            return ((remainder(lhs, rhs) != 0) && ((remainder(lhs, rhs) < 0) != (rhs < 0)))
                         ? division_neg(lhs, rhs)
                         : division(lhs, rhs);
        }
    };

    template<_impl::shift_op Operator, tag RhsTag, typename Lhs, typename Rhs>
    struct custom_operator<Operator, op_value<Lhs, neg_inf_rounding_tag>, op_value<Rhs, RhsTag>> : Operator {
    };

    template<_impl::prefix_op Operator, typename Rhs>
    struct custom_operator<Operator, op_value<Rhs, neg_inf_rounding_tag>> : Operator {
    };

    template<_impl::postfix_op Operator, typename Lhs>
    struct custom_operator<Operator, op_value<Lhs, neg_inf_rounding_tag>> : Operator {
    };
}

#endif  // CNL_IMPL_ROUNDING_FLOOR_ROUNDING_TAG_H
