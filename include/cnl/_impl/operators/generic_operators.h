
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief a place to centralize most duplicate arithmetic operator boilerplate

#if !defined(CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H)
#define CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H

#include "native_tag.h"
#include "operators.h"
#include "../type_traits/enable_if.h"
#include "../../limits.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops

        // true iff T wants generic operator overloads
        template<class T, class Enable = void>
        struct wants_generic_ops : std::false_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // generic operators

        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct shift_operator;

        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct comparison_operator;

        template<class Operator, class RhsOperand, class Enable = void>
        struct pre_operator;

        template<class Operator, class LhsOperand, class Enable = void>
        struct post_operator;

        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct compound_assignment_operator {
            CNL_NODISCARD constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
            {
                return lhs = static_cast<LhsOperand>(
                        cnl::binary_operator<native_tag, typename Operator::binary, LhsOperand, RhsOperand>()(
                                lhs, rhs));
            }
        };

        template<class Operator, class LhsOperand, class RhsOperand, class Enable = void>
        struct compound_assignment_shift_operator {
            CNL_NODISCARD constexpr LhsOperand& operator()(LhsOperand& lhs, RhsOperand const& rhs) const
            {
                return lhs = static_cast<LhsOperand>(
                        shift_operator<typename Operator::binary, LhsOperand, RhsOperand>()(lhs, rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::enable_unary_t

        template<class Operand, class T>
        using enable_unary_t = enable_if_t<_impl::wants_generic_ops<Operand>::value, T>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::enable_binary_t

        template<class LhsOperand, class RhsOperand>
        struct enable_binary;

        template<class LhsOperand, int LhsSize, class RhsOperand>
        struct enable_binary<LhsOperand[LhsSize], RhsOperand> : std::false_type {
        };

        template<class LhsOperand, class RhsOperand, int RhsSize>
        struct enable_binary<LhsOperand, RhsOperand[RhsSize]> : std::false_type {
        };

        template<class LhsOperand, class RhsOperand>
        struct enable_binary
                : std::integral_constant<
                        bool,
                        (numeric_limits<LhsOperand>::is_specialized && numeric_limits<RhsOperand>::is_specialized)
                                && (_impl::wants_generic_ops<LhsOperand>::value
                                        || _impl::wants_generic_ops<RhsOperand>::value)> {
        };

        template<class LhsOperand, class RhsOperand, class T>
        using enable_binary_t = _impl::enable_if_t<enable_binary<LhsOperand, RhsOperand>::value, T>;

        ////////////////////////////////////////////////////////////////////////////////
        // operator overloads

        // unary operators
#define CNL_DEFINE_UNARY_OPERATOR(OP, NAME) \
        template<class Operand> \
        CNL_NODISCARD constexpr auto operator OP (Operand const& operand) \
        -> decltype(cnl::unary_operator<native_tag, enable_unary_t< \
                Operand, NAME>, Operand>()(operand)) \
        { \
            return cnl::unary_operator<native_tag, NAME, Operand>()(operand); \
        }

        CNL_DEFINE_UNARY_OPERATOR(+, plus_op)

        CNL_DEFINE_UNARY_OPERATOR(-, minus_op)

        CNL_DEFINE_UNARY_OPERATOR(~, bitwise_not_op)

        // binary operators
#define CNL_DEFINE_BINARY_OPERATOR(OP, NAME) \
        template<class LhsOperand, class RhsOperand> \
        CNL_NODISCARD constexpr auto operator OP (LhsOperand const& lhs, RhsOperand const& rhs) \
        -> decltype(cnl::binary_operator<enable_binary_t< \
                LhsOperand, RhsOperand, native_tag>, NAME, LhsOperand, RhsOperand>()(lhs, rhs)) \
        { \
            return cnl::binary_operator<native_tag, NAME, LhsOperand, RhsOperand>{}(lhs, rhs); \
        }

        CNL_DEFINE_BINARY_OPERATOR(+, add_op)

        CNL_DEFINE_BINARY_OPERATOR(-, subtract_op)

        CNL_DEFINE_BINARY_OPERATOR(*, multiply_op)

        CNL_DEFINE_BINARY_OPERATOR(/, divide_op)

        CNL_DEFINE_BINARY_OPERATOR(%, modulo_op)

        CNL_DEFINE_BINARY_OPERATOR(|, bitwise_or_op)

        CNL_DEFINE_BINARY_OPERATOR(&, bitwise_and_op)

        CNL_DEFINE_BINARY_OPERATOR(^, bitwise_xor_op)

        // binary operators
#define CNL_DEFINE_SHIFT_OPERATOR(OP, NAME) \
        template<class LhsOperand, class RhsOperand> \
        CNL_NODISCARD constexpr auto operator OP (LhsOperand const& lhs, RhsOperand const& rhs) \
        -> decltype(shift_operator<enable_binary_t< \
                LhsOperand, RhsOperand, NAME>, LhsOperand, RhsOperand>()(lhs, rhs)) \
        { \
            return shift_operator<NAME, LhsOperand, RhsOperand>()(lhs, rhs); \
        }

        CNL_DEFINE_SHIFT_OPERATOR(<<, shift_left_op)

        CNL_DEFINE_SHIFT_OPERATOR(>>, shift_right_op)

        // comparison operators
#define CNL_DEFINE_COMPARISON_OPERATOR(OP, NAME) \
        template<class LhsOperand, class RhsOperand> \
        CNL_NODISCARD constexpr auto operator OP (LhsOperand const& lhs, RhsOperand const& rhs) \
        -> decltype(comparison_operator<enable_binary_t< \
                LhsOperand, RhsOperand, NAME>, LhsOperand, RhsOperand>()(lhs, rhs)) \
        { \
            return comparison_operator<NAME, LhsOperand, RhsOperand>()(lhs, rhs); \
        }

        CNL_DEFINE_COMPARISON_OPERATOR(==, equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(!=, not_equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(<, less_than_op)

        CNL_DEFINE_COMPARISON_OPERATOR(>, greater_than_op)

        CNL_DEFINE_COMPARISON_OPERATOR(<=, less_than_or_equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(>=, greater_than_or_equal_op)

        // pre increment/decrement
#define CNL_DEFINE_PRE_OPERATOR(OP, NAME) \
        template<class RhsOperand> \
        constexpr auto operator OP (RhsOperand& rhs) \
        -> decltype(pre_operator<NAME, RhsOperand>()(rhs)) \
        { \
            return pre_operator<NAME, RhsOperand>()(rhs); \
        }

        CNL_DEFINE_PRE_OPERATOR(++, pre_increment_op)

        CNL_DEFINE_PRE_OPERATOR(--, pre_decrement_op)

        // post increment/decrement
#define CNL_DEFINE_POST_OPERATOR(OP, NAME) \
        template<class LhsOperand> \
        constexpr auto operator OP (LhsOperand& lhs, int) \
        -> decltype(post_operator<NAME, LhsOperand>()(lhs)) \
        { \
            return post_operator<NAME, LhsOperand>()(lhs); \
        }

        CNL_DEFINE_POST_OPERATOR(++, post_increment_op)

        CNL_DEFINE_POST_OPERATOR(--, post_decrement_op)

        // compound assignment operators
#define CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(OP, NAME) \
        template<class LhsOperand, class RhsOperand> \
        constexpr auto operator OP (LhsOperand& lhs, RhsOperand const& rhs) \
        -> enable_binary_t<LhsOperand, RhsOperand, decltype( \
                compound_assignment_operator<NAME, LhsOperand, RhsOperand>()(lhs, rhs))> \
        { \
            return compound_assignment_operator<NAME, LhsOperand, RhsOperand>()(lhs, rhs); \
        }

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(+=, assign_add_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(-=, assign_subtract_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(*=, assign_multiply_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(/=, assign_divide_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(%=, assign_modulo_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(|=, assign_bitwise_or_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(&=, assign_bitwise_and_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(^=, assign_bitwise_xor_op)

        // compound assignment shift operators
#define CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(OP, NAME) \
        template<class LhsOperand, class RhsOperand> \
        constexpr auto operator OP (LhsOperand& lhs, RhsOperand const& rhs) \
        -> enable_binary_t<LhsOperand, RhsOperand, decltype( \
                compound_assignment_shift_operator<NAME, LhsOperand, RhsOperand>()(lhs, rhs))> \
        { \
            return compound_assignment_shift_operator<NAME, LhsOperand, RhsOperand>()( \
                    lhs, rhs); \
        }

        CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(<<=, assign_shift_left_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(>>=, assign_shift_right_op)
    }
}

#endif  // CNL_IMPL_OPERATORS_GENERIC_OPERATORS_H
