
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::scaled_integer` operators

#if !defined(CNL_IMPL_SCALED_INTEGER_OPERATORS_H)
#define CNL_IMPL_SCALED_INTEGER_OPERATORS_H

#include "../scaled/power.h"
#include "is_scaled_integer.h"
#include "type.h"

#include <numeric>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different scaled_integer specializations

    template<typename Operator, typename Rep, int Exponent, int Radix>
    struct unary_operator<Operator, _impl::native_tag, scaled_integer<Rep, power<Exponent, Radix>>> {
        CNL_NODISCARD constexpr auto operator()(scaled_integer<Rep, power<Exponent, Radix>> const& rhs) const
        {
            return _impl::from_rep<scaled_integer<decltype(Operator()(_impl::to_rep(rhs))), power<Exponent, Radix>>>(
                    Operator()(_impl::to_rep(rhs)));
        }
    };

    // comparison between operands with different rep
    template<typename Operator, typename LhsRep, typename RhsRep, class Scale>
    struct comparison_operator<Operator,
            scaled_integer<LhsRep, Scale>,
            scaled_integer<RhsRep, Scale>> {
        CNL_NODISCARD constexpr auto operator()(
                scaled_integer<LhsRep, Scale> const& lhs,
                scaled_integer<RhsRep, Scale> const& rhs) const
        {
            return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };

    // comparison between operands with different rep and exponent
    template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
    struct comparison_operator<Operator,
            scaled_integer<LhsRep, power<LhsExponent, Radix>>,
            scaled_integer<RhsRep, power<RhsExponent, Radix>>,
            _impl::enable_if_t<(LhsExponent<RhsExponent)>> {
        static constexpr int shiftage = RhsExponent - LhsExponent;
        using lhs_type = scaled_integer<LhsRep, power<LhsExponent, Radix>>;
        using rhs_type = scaled_integer<
                decltype(std::declval<RhsRep>()<<constant<shiftage>()),
                power<LhsExponent, Radix>>;
        using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;

        CNL_NODISCARD constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, Radix>> const& lhs,
                scaled_integer<RhsRep, power<RhsExponent, Radix>> const& rhs) const
        {
            return operator_type{}(lhs, rhs);
        }
    };

    template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
    struct comparison_operator<Operator,
            scaled_integer<LhsRep, power<LhsExponent, Radix>>,
            scaled_integer<RhsRep, power<RhsExponent, Radix>>,
            _impl::enable_if_t<(RhsExponent<LhsExponent)>> {
        static constexpr int shiftage = LhsExponent - RhsExponent;
        using lhs_type = scaled_integer<
                decltype(std::declval<LhsRep>()<<constant<shiftage>()),
                power<RhsExponent, Radix>>;
        using rhs_type = scaled_integer<RhsRep, power<RhsExponent, Radix>>;
        using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;

        CNL_NODISCARD constexpr auto operator()(
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
    template<class Operator, typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    struct shift_operator<
            Operator,
            _impl::native_tag, _impl::native_tag,
            scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>, Rhs,
            _impl::enable_if_t<!_impl::is_constant<Rhs>::value>> {
        using lhs_type = scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>;
        CNL_NODISCARD constexpr auto operator()(lhs_type const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<lhs_type>(Operator{}(_impl::to_rep(lhs), rhs));
        }
    };

    // scaled_integer << constant
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct shift_operator<
            _impl::shift_left_op,
            _impl::native_tag, _impl::native_tag,
            scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>, constant<RhsValue>> {
        using result_type = scaled_integer<LhsRep, power<LhsExponent+int(RhsValue), LhsRadix>>;
        CNL_NODISCARD constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs, constant<RhsValue>) const
        {
            return _impl::from_rep<result_type>(_impl::to_rep(lhs));
        };
    };

    // scaled_integer >> constant
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    struct shift_operator<
            _impl::shift_right_op,
            _impl::native_tag, _impl::native_tag,
            scaled_integer<LhsRep, power<LhsExponent, LhsRadix>>, constant<RhsValue>> {
        using result_type = scaled_integer<LhsRep, power<LhsExponent-int(RhsValue), LhsRadix>>;
        CNL_NODISCARD constexpr auto operator()(
                scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs, constant<RhsValue>) const
        {
            return _impl::from_rep<result_type>(_impl::to_rep(lhs));
        };
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_OPERATORS_H
