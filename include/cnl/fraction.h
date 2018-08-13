
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fraction` type

#if !defined(CNL_FRACTION_H)
#define CNL_FRACTION_H 1

#include "_impl/type_traits/enable_if.h"

#if defined(__cpp_lib_experimental_gcd_lcm)
#include <experimental/numeric>
#endif

/// compositional numeric library
namespace cnl {

    /// \brief numeric type represented as the fraction, \ref numerator `/` \ref denominator
    ///
    /// \tparam Numerator the type of numerator
    /// \tparam Exponent the type of denominator

    template<typename Numerator, typename Denominator = int>
    struct fraction {
        /// alias to `Numerator`
        using numerator_type = Numerator;

        /// alias to `Denominator`
        using denominator_type = Denominator;

        explicit constexpr fraction(Numerator const& n, Denominator const& d)
                : numerator{n}, denominator{d} {}

        explicit constexpr fraction(Numerator const& n)
                : numerator{n}, denominator{1} {}

        /// returns the quotient, \ref numerator `/` \ref denominator
        template<typename Scalar, _impl::enable_if_t<std::is_floating_point<Scalar>::value, int> = 0>
        explicit constexpr operator Scalar() const
        {
            return static_cast<Scalar>(numerator)/static_cast<Scalar>(denominator);
        }

        /// the numerator (top number) of the fraction
        numerator_type numerator;

        /// the denominator (bottom number) of the fraction
        denominator_type denominator = 1;
    };

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
