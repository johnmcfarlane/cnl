
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
    namespace _impl {
        template<typename Numerator, typename Denominator>
        constexpr auto one(fraction<Numerator, Denominator> const& f)
        -> decltype(f.numerator==f.denominator)
        {
            return f.numerator==f.denominator;
        }
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator==(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_impl::one(lhs/rhs))
    {
        return _impl::one(lhs/rhs);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator!=(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(!(lhs==rhs))
    {
        return !(lhs==rhs);
    }
}

#endif  // CNL_IMPL_FRACTION_OPERATORS_H
