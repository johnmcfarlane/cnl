
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::fixed_point` operators

#if !defined(CNL_FIXED_POINT_OPERATORS_H)
#define CNL_FIXED_POINT_OPERATORS_H 1

#include "fixed_point_arithmetic.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) arithmetic operators

    // negate
    template<class RhsRep, int RhsExponent>
    constexpr auto operator-(fixed_point<RhsRep, RhsExponent> const& rhs)
    -> fixed_point<decltype(-_impl::to_rep(rhs)), RhsExponent>
    {
        using result_type = fixed_point<decltype(-_impl::to_rep(rhs)), RhsExponent>;
        return _impl::from_rep<result_type>(-_impl::to_rep(rhs));
    }

    // add
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            fixed_point<LhsRep, LhsExponent> const& lhs,
            fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::add_tag);
    }

    // subtract
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            fixed_point<LhsRep, LhsExponent> const& lhs,
            fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::subtract_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::subtract_tag);
    }

    // multiply
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            fixed_point<LhsRep, LhsExponent> const& lhs,
            fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::multiply_tag))
    {
        return _impl::fp::operate<_impl::fp::arithmetic_operator_tag>(lhs, rhs, _impl::multiply_tag);
    }

    // divide
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            fixed_point<LhsRep, LhsExponent> const& lhs,
            fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs, rhs, _impl::divide_tag))
    {
        return _impl::fp::operate<_impl::fp::division_arithmetic_operator_tag>(lhs, rhs, _impl::divide_tag);
    }

    // modulo
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator%(
            fixed_point<LhsRep, LhsExponent> const& lhs,
            fixed_point<RhsRep, RhsExponent> const& rhs)
    -> fixed_point<decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)), LhsExponent>
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)), LhsExponent>>(
                _impl::to_rep(lhs)%_impl::to_rep(rhs));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    namespace _fixed_point_operators_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_heterogeneous() {
            return (!std::is_same<Lhs, Rhs>::value) &&
                   (_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value);
        }
    }

    namespace _impl {
        template<
                class Operator, class Lhs, class Rhs,
                class = _impl::enable_if_t<Operator::is_comparison && cnl::_fixed_point_operators_impl::is_heterogeneous<Lhs, Rhs>()>>
        constexpr auto operate(Lhs const& lhs, Rhs const& rhs, Operator op)
        -> decltype(op(static_cast<_impl::common_type_t<Lhs, Rhs>>(lhs), static_cast<_impl::common_type_t<Lhs, Rhs>>(rhs)))
        {
            return op(static_cast<_impl::common_type_t<Lhs, Rhs>>(lhs), static_cast<_impl::common_type_t<Lhs, Rhs>>(rhs));
        };

        template<class Operator, class Rep, int Exponent, class = _impl::enable_if_t<Operator::is_comparison>>
        constexpr auto operate(fixed_point<Rep, Exponent> const& lhs, fixed_point<Rep, Exponent> const& rhs, Operator op)
        -> decltype(op(_impl::to_rep(lhs), _impl::to_rep(rhs)))
        {
            return op(_impl::to_rep(lhs), _impl::to_rep(rhs));
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ non-fixed_point) arithmetic operators

    // fixed-point, integer -> fixed-point
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator+(fixed_point<LhsRep, LhsExponent> const& lhs, RhsInteger const& rhs)
    -> decltype(lhs + fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs + fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator-(fixed_point<LhsRep, LhsExponent> const& lhs, RhsInteger const& rhs)
    -> decltype(lhs - fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs - fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator*(fixed_point<LhsRep, LhsExponent> const& lhs, RhsInteger const& rhs)
    -> decltype(lhs*fixed_point<RhsInteger>(rhs))
    {
        return lhs*fixed_point<RhsInteger>(rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator/(fixed_point<LhsRep, LhsExponent> const& lhs, RhsInteger const& rhs)
    -> decltype(lhs/fixed_point<RhsInteger>{rhs})
    {
        return lhs/fixed_point<RhsInteger>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>>
    constexpr auto operator%(fixed_point<LhsRep, LhsExponent> const& lhs, RhsInteger const& rhs)
    -> decltype(lhs%fixed_point<RhsInteger>{rhs})
    {
        return lhs%fixed_point<RhsInteger>{rhs};
    }

    // integer. fixed-point -> fixed-point
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator+(LhsInteger const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(fixed_point<LhsInteger, 0>{lhs} + rhs)
    {
        return fixed_point<LhsInteger, 0>{lhs} + rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator-(LhsInteger const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}-rhs)
    {
        return fixed_point<LhsInteger>{lhs}-rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator*(LhsInteger const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}*rhs)
    {
        return fixed_point<LhsInteger>{lhs}*rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator/(LhsInteger const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}/rhs)
    {
        return fixed_point<LhsInteger>{lhs}/rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = _impl::enable_if_t<numeric_limits<LhsInteger>::is_integer>>
    constexpr auto operator%(LhsInteger const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}%rhs)
    {
        return fixed_point<LhsInteger>{lhs}%rhs;
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t<numeric_limits<RhsFloat>::is_iec559>>
    constexpr auto operator+(fixed_point<LhsRep, LhsExponent> const& lhs, RhsFloat const& rhs)
    -> decltype(static_cast<RhsFloat>(lhs)+rhs)
    {
        return static_cast<RhsFloat>(lhs)+rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t<numeric_limits<RhsFloat>::is_iec559>>
    constexpr auto operator-(fixed_point<LhsRep, LhsExponent> const& lhs, RhsFloat const& rhs)
    -> decltype(static_cast<RhsFloat>(lhs)-rhs)
    {
        return static_cast<RhsFloat>(lhs)-rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t <numeric_limits<RhsFloat>::is_iec559>>
    constexpr auto operator*(fixed_point<LhsRep, LhsExponent> const& lhs, RhsFloat const& rhs)
    -> decltype(static_cast<RhsFloat>(lhs)*rhs)
    {
        return static_cast<RhsFloat>(lhs)*rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t<numeric_limits<RhsFloat>::is_iec559>>
    constexpr auto operator/(fixed_point<LhsRep, LhsExponent> const& lhs, RhsFloat const& rhs)
    -> decltype(static_cast<RhsFloat>(lhs)/rhs)
    {
        return static_cast<RhsFloat>(lhs)/rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = _impl::enable_if_t<numeric_limits<RhsFloat>::is_iec559>>
    constexpr auto operator%(fixed_point<LhsRep, LhsExponent> const& lhs, RhsFloat const& rhs)
    -> decltype(static_cast<RhsFloat>(lhs)%rhs)
    {
        return static_cast<RhsFloat>(lhs)%rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t <numeric_limits<LhsFloat>::is_iec559>>
    constexpr auto operator+(LhsFloat const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(lhs+static_cast<LhsFloat>(rhs))
    {
        return lhs+static_cast<LhsFloat>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t<numeric_limits<LhsFloat>::is_iec559>>
    constexpr auto operator-(LhsFloat const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(lhs-static_cast<LhsFloat>(rhs))
    {
        return lhs-static_cast<LhsFloat>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t<numeric_limits<LhsFloat>::is_iec559>>
    constexpr auto operator*(LhsFloat const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(lhs*static_cast<LhsFloat>(rhs))
    {
        return lhs*static_cast<LhsFloat>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t<numeric_limits<LhsFloat>::is_iec559>>
    constexpr auto operator/(LhsFloat const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(lhs/static_cast<LhsFloat>(rhs))
    {
        return lhs/static_cast<LhsFloat>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = _impl::enable_if_t<numeric_limits<LhsFloat>::is_iec559>>
    constexpr auto operator%(LhsFloat const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs)
    -> decltype(lhs%static_cast<LhsFloat>(rhs))
    {
        return lhs%static_cast<LhsFloat>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // fixed_point, dynamic
    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << rhs), LhsExponent>>(_impl::to_rep(lhs) << rhs))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << rhs), LhsExponent>>(_impl::to_rep(lhs) << rhs);
    }

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> rhs), LhsExponent>>(_impl::to_rep(lhs) >> rhs))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> rhs), LhsExponent>>(_impl::to_rep(lhs) >> rhs);
    }

    // fixed_point, const_integer
    template<class LhsRep, int LhsExponent, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+RhsValue>
    operator<<(fixed_point<LhsRep, LhsExponent> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent+RhsValue>>(_impl::to_rep(lhs));
    }

    template<class LhsRep, int LhsExponent, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-RhsValue>
    operator>>(fixed_point<LhsRep, LhsExponent> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent-RhsValue>>(_impl::to_rep(lhs));
    }
}

#endif  // CNL_FIXED_POINT_OPERATORS_H
