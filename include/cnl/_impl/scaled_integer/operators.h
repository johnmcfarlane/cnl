
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::scaled_integer` operators

#if !defined(CNL_IMPL_SCALED_INTEGER_OPERATORS_H)
#define CNL_IMPL_SCALED_INTEGER_OPERATORS_H 1

#include "is_scaled_integer.h"
#include "type.h"
#include "../scaled/power.h"

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
        -> decltype(_impl::from_rep<scaled_integer<decltype(Operator()(_impl::to_rep(rhs))), power<Exponent, Radix>>>(
                Operator()(_impl::to_rep(rhs))))
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
        -> decltype(Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs)))
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
        -> decltype(operator_type{}(lhs, rhs))
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
        -> decltype(operator_type{}(lhs, rhs))
        {
            return operator_type{}(lhs, rhs);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // pre-increment/decrement arithmetic operators

    template<typename Operator, typename Rep, int Exponent, int Radix>
    struct pre_operator<Operator, _impl::native_tag, scaled_integer<Rep, power<Exponent, Radix>>> {
        CNL_NODISCARD constexpr auto operator()(scaled_integer<Rep, power<Exponent, Radix>>& rhs) const
        -> decltype(typename _impl::pre_to_assign<Operator>::type{}(rhs, 1))
        {
            return typename _impl::pre_to_assign<Operator>::type{}(rhs, 1);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // post-increment/decrement arithmetic operators

    template<typename Operator, typename Rep, int Exponent, int Radix>
    struct post_operator<Operator, _impl::native_tag, scaled_integer<Rep, power<Exponent, Radix>>> {
        CNL_RELAXED_CONSTEXPR scaled_integer<Rep, power<Exponent, Radix>> operator()(scaled_integer<Rep, power<Exponent, Radix>>& rhs) const
        {
            auto copy = rhs;
            typename _impl::post_to_assign<Operator>::type{}(rhs, 1);
            return copy;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // scaled_integer, dynamic
    template<typename LhsRep, class LhsScale, typename Rhs>
    CNL_NODISCARD constexpr auto operator<<(scaled_integer<LhsRep, LhsScale> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<scaled_integer<decltype(_impl::to_rep(lhs) << int(rhs)), LhsScale>>(
            _impl::to_rep(lhs) << int(rhs)))
    {
        return _impl::from_rep<scaled_integer<decltype(_impl::to_rep(lhs) << int(rhs)), LhsScale>>(
                _impl::to_rep(lhs) << int(rhs));
    }

    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    CNL_NODISCARD constexpr auto operator>>(
            scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs,
            Rhs const& rhs)
    -> decltype(_impl::from_rep<scaled_integer<
            decltype(_impl::to_rep(lhs) >> int(rhs)),
            power<LhsExponent, LhsRadix>>>(_impl::to_rep(lhs) >> int(rhs)))
    {
        return _impl::from_rep<scaled_integer<
                decltype(_impl::to_rep(lhs) >> int(rhs)),
                power<LhsExponent, LhsRadix>>>(_impl::to_rep(lhs) >> int(rhs));
    }

    // scaled_integer, const_integer
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    CNL_NODISCARD constexpr scaled_integer<LhsRep, power<LhsExponent+static_cast<int>(RhsValue), LhsRadix>>
    operator<<(scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<scaled_integer<LhsRep, power<LhsExponent+static_cast<int>(RhsValue), LhsRadix>>>(
                _impl::to_rep(lhs));
    }

    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    CNL_NODISCARD constexpr scaled_integer<LhsRep, power<LhsExponent-static_cast<int>(RhsValue), LhsRadix>>
    operator>>(scaled_integer<LhsRep, power<LhsExponent, LhsRadix>> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<scaled_integer<LhsRep, power<LhsExponent-static_cast<int>(RhsValue), LhsRadix>>>(
                _impl::to_rep(lhs));
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_OPERATORS_H
