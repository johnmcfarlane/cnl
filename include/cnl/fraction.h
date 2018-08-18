
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fraction` type

#if !defined(CNL_FRACTION_H)
#define CNL_FRACTION_H 1

#include "_impl/fraction/type.h"

#if defined(__cpp_lib_experimental_gcd_lcm)
#include <experimental/numeric>
#endif

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_fraction

    /// creates a fraction with types deduced from the numerator and denominator
    template<typename Numerator, typename Denominator>
    constexpr fraction<Numerator, Denominator> make_fraction(Numerator const& n, Denominator const& d)
    {
        return fraction<Numerator, Denominator>{n, d};
    }

    /// creates a fraction with types deduced from the numerator
    template<typename Numerator>
    constexpr fraction<Numerator, Numerator> make_fraction(Numerator const& n)
    {
        return fraction<Numerator, Numerator>{n, 1};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::gcd

#if defined(__cpp_lib_experimental_gcd_lcm)
    template<typename Numerator, typename Denominator>
    constexpr auto gcd(fraction<Numerator, Denominator> const& f) {
        return std::experimental::gcd(f.numerator, f.denominator);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::reduce

    template<typename Numerator, typename Denominator>
    constexpr auto reduce(fraction<Numerator, Denominator> const& f) {
        return make_fraction(f.numerator / gcd(f), f.denominator / gcd(f));
    }
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fraction operators

    // cnl::fraction arithmetic
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fraction(
            lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fraction(
                lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fraction(
            lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fraction(
                lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator*(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator))
    {
        return make_fraction(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator/(
            fraction<LhsNumerator, LhsDenominator> const& lhs,
            fraction<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator))
    {
        return make_fraction(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator);
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

#endif  // CNL_FRACTION_H
