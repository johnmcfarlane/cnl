
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::scaled_integer` operators

#if !defined(CNL_IMPL_SCALED_INTEGER_OPERATORS_H)
#define CNL_IMPL_SCALED_INTEGER_OPERATORS_H

#include "../scaled/power.h"
#include "definition.h"

#include <numeric>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads

    template<_impl::unary_arithmetic_op Operator, typename Rep, int Exponent, int Radix>
    struct custom_operator<
            Operator, op_value<scaled_integer<Rep, power<Exponent, Radix>>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<Rep, power<Exponent, Radix>> const& rhs) const
        {
            return _impl::from_rep<scaled_integer<
                    decltype(Operator()(_impl::to_rep(rhs))), power<Exponent, Radix>>>(
                    Operator()(_impl::to_rep(rhs)));
        }
    };

    // comparison between operands with different rep
    template<_impl::comparison_op Operator, typename LhsRep, typename RhsRep, class Scale>
    struct custom_operator<
            Operator, op_value<scaled_integer<LhsRep, Scale>>, op_value<scaled_integer<RhsRep, Scale>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<LhsRep, Scale> const& lhs,
                scaled_integer<RhsRep, Scale> const& rhs) const
        {
            return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    // comparison between operands with different rep and exponent
    template<
            _impl::comparison_op Operator,
            typename LhsRep, int LhsExponent,
            typename RhsRep, int RhsExponent,
            int Radix>
    requires(LhsExponent < RhsExponent) struct custom_operator<
            Operator,
            op_value<scaled_integer<LhsRep, power<LhsExponent, Radix>>>,
            op_value<scaled_integer<RhsRep, power<RhsExponent, Radix>>>> {
        using result_tag = _impl::native_tag;

        static constexpr int shiftage = RhsExponent - LhsExponent;
        using lhs_type = scaled_integer<LhsRep, power<LhsExponent, Radix>>;
        using rhs_type = scaled_integer<
                decltype(std::declval<RhsRep>() << constant<shiftage>()),
                power<LhsExponent, Radix>>;
        using operator_type = custom_operator<Operator, op_value<lhs_type>, op_value<rhs_type>>;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, Radix>> const& lhs,
                scaled_integer<RhsRep, power<RhsExponent, Radix>> const& rhs) const
        {
            return operator_type{}(lhs, rhs);
        }
    };

    template<
            _impl::comparison_op Operator,
            typename LhsRep, int LhsExponent,
            typename RhsRep, int RhsExponent,
            int Radix>
    requires(RhsExponent < LhsExponent) struct custom_operator<
            Operator,
            op_value<scaled_integer<LhsRep, power<LhsExponent, Radix>>>,
            op_value<scaled_integer<RhsRep, power<RhsExponent, Radix>>>> {
        using result_tag = _impl::native_tag;

        static constexpr int shiftage = LhsExponent - RhsExponent;
        using lhs_type = scaled_integer<
                decltype(std::declval<LhsRep>() << constant<shiftage>()),
                power<RhsExponent, Radix>>;
        using rhs_type = scaled_integer<RhsRep, power<RhsExponent, Radix>>;
        using operator_type = custom_operator<Operator, op_value<lhs_type>, op_value<rhs_type>>;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, Radix>> const& lhs,
                scaled_integer<RhsRep, power<RhsExponent, Radix>> const& rhs) const
        {
            return operator_type{}(lhs, rhs);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // scaled_integer << non-constant
    // scaled_integer >> non-constant
    template<_impl::shift_op Operator, typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    requires(!_impl::is_constant<Rhs>::value) struct custom_operator<
            Operator,
            op_value<scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>>,
            op_value<Rhs>> {
        using result_tag = _impl::native_tag;

        using lhs_type = scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>;
        [[nodiscard]] constexpr auto operator()(lhs_type const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<lhs_type>(Operator{}(_impl::to_rep(lhs), rhs));
        }
    };

    // scaled_integer << constant
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct custom_operator<
            _impl::shift_left_op,
            op_value<scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>>,
            op_value<constant<RhsValue>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs,
                constant<RhsValue>) const
        {
            using result_type = scaled_integer<LhsRep, power<LhsExponent + int(RhsValue), LhsRadix>>;
            return _impl::from_rep<result_type>(_impl::to_rep(lhs));
        };
    };

    // scaled_integer >> constant
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct custom_operator<
            _impl::shift_right_op,
            op_value<scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>>,
            op_value<constant<RhsValue>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs,
                constant<RhsValue>) const
        {
            using result_type = scaled_integer<LhsRep, power<LhsExponent - int(RhsValue), LhsRadix>>;
            return _impl::from_rep<result_type>(_impl::to_rep(lhs));
        };
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_OPERATORS_H
