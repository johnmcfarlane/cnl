
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief primary templates providing basis for arithmetic operator customization points

#if !defined(CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H)
#define CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H

#include "../config.h"
#include "../num_traits/tag.h"
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

        ////////////////////////////////////////////////////////////////////////////////
        // generic operators
    }

    template<class ArchetypeTag, typename Initializer>
    struct deduction;

    template<
            tag DestTag, tag SrcTag, typename Destination, typename Source,
            typename Enabled = void>
    struct convert_operator;

    template<_impl::unary_arithmetic_op Operator, tag RhsTag, typename Rhs, class Enabled = void>
    struct unary_arithmetic_operator;

    template<
            _impl::binary_arithmetic_op Operator, tag LhsTag, tag RhsTag, typename Lhs, typename Rhs,
            class Enabled = void>
    struct binary_arithmetic_operator;

    template<
            _impl::shift_op Operator, tag LhsTag, tag RhsTag, class LhsOperand, class RhsOperand,
            class Enable = void>
    struct shift_operator;

    template<_impl::comparison_op Operator, class LhsOperand, class RhsOperand, class Enable = void>
    struct comparison_operator;

    template<_impl::prefix_op Operator, tag RhsTag, class RhsOperand, class Enable = void>
    struct prefix_operator;

    template<_impl::postfix_op Operator, tag LhsTag, class LhsOperand, class Enable = void>
    struct postfix_operator;

    template<
            _impl::assign_op Operator, tag LhsTag, tag RhsTag, class LhsOperand, class RhsOperand,
            class Enable = void>
    struct compound_assignment_operator {
        constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
        {
            using binary_arithmetic_operator = cnl::binary_arithmetic_operator<
                    typename Operator::binary, LhsTag, RhsTag, LhsOperand, RhsOperand>;
            using binary_arithmetic_operator_result = decltype(binary_arithmetic_operator{}(lhs, rhs));
            using convert_operator =
                    cnl::convert_operator<LhsTag, RhsTag, LhsOperand, binary_arithmetic_operator_result>;
            return lhs = convert_operator{}(binary_arithmetic_operator{}(lhs, rhs));
        }
    };

    template<
            _impl::assign_shift_op Operator, tag LhsTag, tag RhsTag, class LhsOperand, class RhsOperand,
            class Enable = void>
    struct compound_assignment_shift_operator {
        constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
        {
            using shift_operator = cnl::shift_operator<
                    typename Operator::binary, LhsTag, RhsTag, LhsOperand, RhsOperand>;
            using shift_operator_result = decltype(shift_operator{}(lhs, rhs));
            using convert_operator =
                    cnl::convert_operator<LhsTag, RhsTag, LhsOperand, shift_operator_result>;
            return lhs = convert_operator{}(shift_operator{}(lhs, rhs));
        }
    };
}

#endif  // CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H
