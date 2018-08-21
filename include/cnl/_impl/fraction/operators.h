
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_OPERATORS_H)
#define CNL_IMPL_FRACTION_OPERATORS_H 1

#include "make_fraction.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    // cnl::fraction arithmetic
    template<typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(+rhs.numerator, +rhs.denominator))
    {
        return _impl::make_fraction(+rhs.numerator, +rhs.denominator);
    }

    template<typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(-rhs.numerator, -rhs.denominator))
    {
        return _impl::make_fraction(-rhs.numerator, -rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(
            lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(
                lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(
            lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(
                lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator*(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator))
    {
        return _impl::make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator/(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator))
    {
        return _impl::make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator);
    }

    // cnl::fraction comparison
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator==(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator==rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator==rhs.numerator*lhs.denominator;
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator!=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator!=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator!=rhs.numerator*lhs.denominator;
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator<(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator<rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator<rhs.numerator*lhs.denominator;
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator>(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator>rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator>rhs.numerator*lhs.denominator;
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator<=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator<=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator<=rhs.numerator*lhs.denominator;
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator>=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(lhs.numerator*rhs.denominator>=rhs.numerator*lhs.denominator)
    {
        return lhs.numerator*rhs.denominator>=rhs.numerator*lhs.denominator;
    }
}

#endif  // CNL_IMPL_FRACTION_OPERATORS_H
