
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::fixed_point` operators

#if !defined(CNL_FIXED_POINT_OPERATORS_H)
#define CNL_FIXED_POINT_OPERATORS_H 1

#include "arithmetic.h"

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
        template<class Operator, class Rep, int Exponent>
        struct comparison_operator<Operator, fixed_point<Rep, Exponent>, fixed_point<Rep, Exponent>> {
            constexpr auto operator()(
                    fixed_point<Rep, Exponent> const& lhs,
                    fixed_point<Rep, Exponent> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };

        template<class Operator, class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct comparison_operator<Operator, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(Operator()(static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>>(lhs), static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>>(rhs)))
            {
                using common_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>;
                return Operator()(static_cast<common_type>(lhs), static_cast<common_type>(rhs));
            }
        };

        template<class Operator, class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(_impl::fp::operate<_impl::fp::arithmetic::lean_tag>(lhs, rhs, Operator{}))
            {
                return _impl::fp::operate<_impl::fp::arithmetic::lean_tag>(lhs, rhs, Operator{});
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // fixed_point, dynamic
    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent>>(_impl::to_rep(lhs) << int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent>>(_impl::to_rep(lhs) << int(rhs));
    }

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> int(rhs)), LhsExponent>>(_impl::to_rep(lhs) >> int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> int(rhs)), LhsExponent>>(_impl::to_rep(lhs) >> int(rhs));
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
