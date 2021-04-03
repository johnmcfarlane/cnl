
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H

#include "../custom_operator/definition.h"
#include "../custom_operator/native_tag.h"
#include "is_rounding_tag.h"
#include "is_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify round-to-nearest behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag round to the nearest representatable value in
    /// situations where the precise value cannot be represented.
    ///
    /// \headerfile cnl/rounding.h
    /// \sa cnl::rounding_integer, cnl::convert, cnl::native_rounding_tag
    struct nearest_rounding_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_rounding_tag<nearest_rounding_tag> : std::true_type {
        };
    }

    template<_impl::unary_arithmetic_op Operator, typename Operand>
    struct custom_operator<Operator, op_value<Operand, nearest_rounding_tag>>
        : custom_operator<Operator, op_value<Operand, _impl::native_tag>> {
    };

    template<_impl::binary_arithmetic_op Operator, typename Lhs, typename Rhs>
    struct custom_operator<Operator, op_value<Lhs, nearest_rounding_tag>, op_value<Rhs, nearest_rounding_tag>>
        : Operator {
    };

    template<typename Lhs, typename Rhs>
    struct custom_operator<_impl::divide_op, op_value<Lhs, nearest_rounding_tag>, op_value<Rhs, nearest_rounding_tag>> {
        [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
                -> decltype(lhs / rhs)
        {
            return (((lhs < 0) ^ (rhs < 0)) ? lhs - (rhs / 2) : lhs + (rhs / 2)) / rhs;
        }
    };

    template<_impl::shift_op Operator, typename Lhs, typename Rhs, tag RhsTag>
    struct custom_operator<Operator, op_value<Lhs, nearest_rounding_tag>, op_value<Rhs, RhsTag>> : Operator {
    };

    template<_impl::prefix_op Operator, typename Rhs>
    struct custom_operator<Operator, op_value<Rhs, nearest_rounding_tag>> : Operator {
    };

    template<_impl::postfix_op Operator, typename Lhs>
    struct custom_operator<Operator, op_value<Lhs, nearest_rounding_tag>> : Operator {
    };
}

#endif  // CNL_IMPL_ROUNDING_NEAREST_ROUNDING_TAG_H
