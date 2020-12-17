
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief a place to centralize most duplicate arithmetic operator boilerplate

#if !defined(CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H)
#define CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H

#include "../config.h"
#include "operators.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        struct native_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops

        // true iff T wants generic operator overloads
        template<class T, class Enable = void>
        struct wants_generic_ops : std::false_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // generic operators
    }

    template<class ArchetypeTag, typename Initializer, typename Enabled = void>
    struct deduction;

    template<
            class DestTag, class SrcTag, typename Destination, typename Source,
            typename Enabled = void>
    struct convert_operator;

    template<class Operator, class RhsTag, typename Rhs, class Enabled = void>
    struct unary_operator;

    template<
            class Operator, class LhsTag, class RhsTag, typename Lhs, typename Rhs,
            class Enabled = void>
    struct binary_operator;

    template<
            class Operator, class LhsTag, class RhsTag, class LhsOperand, class RhsOperand,
            class Enable = void>
    struct shift_operator;

    template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
    struct comparison_operator;

    template<class Operator, class Tag, class RhsOperand, class Enable = void>
    struct pre_operator;

    template<class Operator, class Tag, class LhsOperand, class Enable = void>
    struct post_operator;

    template<
            class Operator, class LhsTag, class RhsTag, class LhsOperand, class RhsOperand,
            class Enable = void>
    struct compound_assignment_operator {
        constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
        {
            using binary_operator = cnl::binary_operator<
                    typename Operator::binary, LhsTag, RhsTag, LhsOperand, RhsOperand>;
            using binary_operator_result = decltype(binary_operator{}(lhs, rhs));
            using convert_operator =
                    cnl::convert_operator<LhsTag, RhsTag, LhsOperand, binary_operator_result>;
            return lhs = convert_operator{}(binary_operator{}(lhs, rhs));
        }
    };

    template<
            class Operator, class LhsTag, class RhsTag, class LhsOperand, class RhsOperand,
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

#endif // CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H
