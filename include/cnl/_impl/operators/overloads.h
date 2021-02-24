
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief a place to centralize most duplicate arithmetic operator boilerplate

#if !defined(CNL_IMPL_OPERATORS_OVERLOADS_H)
#define CNL_IMPL_OPERATORS_OVERLOADS_H

#include "custom_operator.h"
#include "operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        struct native_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops_binary

        template<typename LhsOperand, typename RhsOperand>
        inline constexpr auto wants_generic_ops_binary =
                _impl::wants_generic_ops<LhsOperand> || _impl::wants_generic_ops<RhsOperand>;

        ////////////////////////////////////////////////////////////////////////////////
        // operator overloads

        // unary operators

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_UNARY_OPERATOR(OP, NAME) \
    template<class Operand> \
    requires _impl::wants_generic_ops<Operand> [[nodiscard]] constexpr auto operator OP(Operand const& rhs) \
    { \
        return cnl::custom_operator<NAME, cnl::op_value<Operand>>()(rhs); \
    }

        CNL_DEFINE_UNARY_OPERATOR(+, plus_op)

        CNL_DEFINE_UNARY_OPERATOR(-, minus_op)

        CNL_DEFINE_UNARY_OPERATOR(~, bitwise_not_op)

        // binary operators

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_BINARY_OPERATOR(OP, NAME) \
    template<class LhsOperand, class RhsOperand> \
    requires wants_generic_ops_binary<LhsOperand, RhsOperand> [[nodiscard]] constexpr auto \
    operator OP(LhsOperand const& lhs, RhsOperand const& rhs) \
    { \
        return cnl::custom_operator<NAME, cnl::op_value<LhsOperand>, cnl::op_value<RhsOperand>>{}( \
                lhs, rhs); \
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

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_SHIFT_OPERATOR(OP, NAME) \
    template<class LhsOperand, class RhsOperand> \
    requires wants_generic_ops_binary<LhsOperand, RhsOperand> [[nodiscard]] constexpr auto \
    operator OP(LhsOperand const& lhs, RhsOperand const& rhs) \
    { \
        return cnl::custom_operator<NAME, op_value<LhsOperand>, op_value<RhsOperand>>()(lhs, rhs); \
    }

        CNL_DEFINE_SHIFT_OPERATOR(<<, shift_left_op)

        CNL_DEFINE_SHIFT_OPERATOR(>>, shift_right_op)

        // comparison operators

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_COMPARISON_OPERATOR(OP, NAME) \
    template<class LhsOperand, class RhsOperand> \
    requires wants_generic_ops_binary<LhsOperand, RhsOperand> [[nodiscard]] constexpr auto \
    operator OP(LhsOperand const& lhs, RhsOperand const& rhs) \
    { \
        return cnl::custom_operator<NAME, op_value<LhsOperand>, op_value<RhsOperand>>()(lhs, rhs); \
    }

        CNL_DEFINE_COMPARISON_OPERATOR(==, equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(!=, not_equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(<, less_than_op)

        CNL_DEFINE_COMPARISON_OPERATOR(>, greater_than_op)

        CNL_DEFINE_COMPARISON_OPERATOR(<=, less_than_or_equal_op)

        CNL_DEFINE_COMPARISON_OPERATOR(>=, greater_than_or_equal_op)

        // pre increment/decrement

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_PRE_OPERATOR(OP, NAME) \
    template<class RhsOperand> \
    constexpr decltype(auto) operator OP(RhsOperand& rhs) \
    { \
        return cnl::custom_operator<NAME, cnl::op_value<RhsOperand>>()(rhs); \
    }

        CNL_DEFINE_PRE_OPERATOR(++, pre_increment_op)

        CNL_DEFINE_PRE_OPERATOR(--, pre_decrement_op)

        // post increment/decrement

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_POST_OPERATOR(OP, NAME) \
    template<class LhsOperand> \
    constexpr auto operator OP(LhsOperand& lhs, int) \
            ->decltype(cnl::custom_operator<NAME, cnl::op_value<LhsOperand>>()(lhs)) \
    { \
        return cnl::custom_operator<NAME, cnl::op_value<LhsOperand>>()(lhs); \
    }

        CNL_DEFINE_POST_OPERATOR(++, post_increment_op)

        CNL_DEFINE_POST_OPERATOR(--, post_decrement_op)

        // compound assignment operators

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_COMPOUND_ASSIGNMENT_OPERATOR(OP, NAME) \
    template<class LhsOperand, class RhsOperand> \
    requires _impl::wants_generic_ops_binary<LhsOperand, RhsOperand> constexpr auto operator OP(LhsOperand& lhs, RhsOperand const& rhs) \
    { \
        return cnl::custom_operator< \
                NAME, op_value<LhsOperand>, op_value<RhsOperand>>()(lhs, rhs); \
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

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(OP, NAME) \
    template<class LhsOperand, class RhsOperand> \
    requires _impl::wants_generic_ops_binary<LhsOperand, RhsOperand> constexpr auto operator OP(LhsOperand& lhs, RhsOperand const& rhs) \
    { \
        return cnl::custom_operator< \
                NAME, op_value<LhsOperand>, op_value<RhsOperand>>()(lhs, rhs); \
    }

        CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(<<=, assign_shift_left_op)

        CNL_DEFINE_COMPOUND_ASSIGNMENT_SHIFT_OPERATOR(>>=, assign_shift_right_op)
    }
}

#endif  // CNL_IMPL_OPERATORS_OVERLOADS_H
