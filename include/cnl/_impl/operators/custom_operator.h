
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief primary templates providing basis for arithmetic operator customization points

#if !defined(CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H)
#define CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H

#include "../../arithmetic.h"
#include "../config.h"
#include "../operators/tag.h"
#include "operators.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        struct native_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops

        // true iff T wants generic operator overloads
        template<class T>
        inline constexpr auto wants_generic_ops = false;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::op_value

    /// \brief operand or result of operation;used as Operands parameter of \ref cnl::custom_operator
    /// \tparam Rep type of value handled by custom_operator specialization
    /// \tparam Tag behavior of `Rep` handled by custom_operator specialization
    template<arithmetic Rep, tag Tag = _impl::native_tag>
    class op_value;

    namespace _impl {
        template<typename T>
        inline constexpr bool is_operand_specialization = false;

        template<arithmetic Rep, typename Tag>
        inline constexpr bool is_operand_specialization<op_value<Rep, Tag>> = true;

        template<typename T>
        concept operand_specialization = is_operand_specialization<T>;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // generic operators

    /// \brief customization point for operator overloads
    /// \note avoids the need to overload every last operator
    template<_impl::op Operator, _impl::operand_specialization... Operands>
    struct custom_operator;

    template<class ArchetypeTag, typename Initializer>
    struct deduction;

    template<_impl::compound_assign_op Operator, class LhsOperand, tag LhsTag, class RhsOperand, tag RhsTag>
    struct custom_operator<
            Operator,
            op_value<LhsOperand, LhsTag>,
            op_value<RhsOperand, RhsTag>> {
        // NOLINTNEXTLINE(modernize-use-trailing-return-type)
        constexpr auto& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
        {
            using compound_assign_operator = cnl::custom_operator<
                    typename Operator::binary, op_value<LhsOperand, LhsTag>, op_value<RhsOperand, RhsTag>>;
            using compound_assign_operator_result = decltype(compound_assign_operator{}(lhs, rhs));
            using convert_operator = cnl::custom_operator<
                    _impl::convert_op,
                    op_value<compound_assign_operator_result, RhsTag>,
                    op_value<LhsOperand, LhsTag>>;
            return lhs = convert_operator{}(compound_assign_operator{}(lhs, rhs));
        }
    };
}

#endif  // CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H
