
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential named definitions related to the `cnl::fixed_point` type

#if !defined(CNL_FIXED_POINT_NAMED_H)
#define CNL_FIXED_POINT_NAMED_H 1

#include "common.h"

#include "fixed_point_arithmetic.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::multiply with fixed_point operand(s)

    /// \brief calculates the product of two \ref fixed_point factors
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param lhs, rhs the factors
    ///
    /// \return product: lhs * rhs
    ///
    /// \note This function multiplies the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa divide

    template<class Lhs, class Rhs>
    constexpr auto multiply(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag))
    {
        return _impl::fp::operate<_impl::fp::named_function_tag>(lhs, rhs, _impl::multiply_tag);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::divide with fixed_point operand(s)

    namespace _divide_impl {
        template<class Lhs, class Rhs>
        struct divide;

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct divide<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(fixed_point<LhsRep, LhsExponent> const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(_impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag)) {
                return _impl::fp::operate<_impl::fp::division_named_function_tag>(lhs, rhs, _impl::divide_tag);
            }
        };

        template<class Lhs, class RhsRep, int RhsExponent>
        struct divide<Lhs, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(Lhs const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(divide<fixed_point<Lhs>, fixed_point<RhsRep, RhsExponent>>()(lhs, rhs)) {
                return divide<fixed_point<Lhs>, fixed_point<RhsRep, RhsExponent>>()(lhs, rhs);
            }
        };

        template<class LhsRep, int LhsExponent, class Rhs>
        struct divide<fixed_point<LhsRep, LhsExponent>, Rhs> {
            constexpr auto operator()(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs) const
            -> decltype(divide<fixed_point<LhsRep, LhsExponent>, fixed_point<Rhs>>()(lhs, rhs)) {
                return divide<fixed_point<LhsRep, LhsExponent>, fixed_point<Rhs>>()(lhs, rhs);
            }
        };

        template<class Lhs, class Rhs>
        struct divide {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(divide<fixed_point<Lhs>, fixed_point<Rhs>>()(lhs, rhs)) {
                return divide<fixed_point<Lhs>, fixed_point<Rhs>>()(lhs, rhs);
            }
        };
    }

    /// \brief calculates the quotient of two \ref fixed_point values
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param lhs, rhs dividend and divisor
    ///
    /// \return quotient: lhs / rhs
    ///
    /// \note This function divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa multiply

    template<class Lhs, class Rhs>
    constexpr auto divide(Lhs const& lhs, Rhs const& rhs)
    -> decltype(_divide_impl::divide<Lhs, Rhs>()(lhs, rhs)) {
        return _divide_impl::divide<Lhs, Rhs>()(lhs, rhs);
    };
}

#endif  // CNL_FIXED_POINT_NAMED_H
