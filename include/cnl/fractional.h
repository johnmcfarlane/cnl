
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::fractional` type

#if !defined(CNL_FRACTIONAL_H)
#define CNL_FRACTIONAL_H 1

#include <cnl/bits/type_traits.h>

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
    struct fractional {
        /// alias to `Numerator`
        using numerator_type = Numerator;

        /// alias to `Denominator`
        using denominator_type = Denominator;

        explicit constexpr fractional(Numerator const& n, Denominator const& d)
                : numerator{n}, denominator{d} {}

        explicit constexpr fractional(Numerator const& n)
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

#if defined(__cpp_deduction_guides)
    // cnl::fractional deduction guides
    template<typename Numerator, typename Denominator>
    fractional(Numerator const& n, Denominator const& d)
    -> fractional<Numerator, Denominator>;

    template<typename Numerator>
    fractional(Numerator const& n)
    -> fractional<Numerator, int>;
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_fractional

    /// creates a fractional value with types deduced from the numerator and denominator
    template<typename Numerator, typename Denominator>
    constexpr fractional<Numerator, Denominator> make_fractional(Numerator const& n, Denominator const& d)
    {
        return fractional<Numerator, Denominator>{n, d};
    }

    /// creates a fractional value with types deduced from the numerator
    template<typename Numerator>
    constexpr fractional<Numerator, Numerator> make_fractional(Numerator const& n)
    {
        return fractional<Numerator, Numerator>{n, 1};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::gcd

#if defined(__cpp_lib_experimental_gcd_lcm)
    template<typename Numerator, typename Denominator>
    constexpr auto gcd(fractional<Numerator, Denominator> const& f) {
        return std::experimental::gcd(f.numerator, f.denominator);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::reduce

    template<typename Numerator, typename Denominator>
    constexpr auto reduce(fractional<Numerator, Denominator> const& f) {
        return make_fractional(f.numerator / gcd(f), f.denominator / gcd(f));
    }
#endif

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fractional operators

    // cnl::fractional arithmetic
    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator+(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(
            lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(
                lhs.numerator*rhs.denominator+rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator-(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(
            lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(
                lhs.numerator*rhs.denominator-rhs.numerator*lhs.denominator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator*(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator))
    {
        return make_fractional(lhs.numerator*rhs.numerator, lhs.denominator*rhs.denominator);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator/(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(make_fractional(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator))
    {
        return make_fractional(lhs.numerator*rhs.denominator, lhs.denominator*rhs.numerator);
    }

    // cnl::fractional comparison
    namespace _fractional_impl {
        template<typename Numerator, typename Denominator>
        constexpr auto one(fractional<Numerator, Denominator> const& f)
        -> decltype(f.numerator==f.denominator)
        {
            return f.numerator==f.denominator;
        }
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator==(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(_fractional_impl::one(lhs/rhs))
    {
        return _fractional_impl::one(lhs/rhs);
    }

    template<typename LhsNumerator, typename LhsDenominator, typename RhsNumerator, typename RhsDenominator>
    constexpr auto operator!=(
            fractional<LhsNumerator, LhsDenominator> const& lhs,
            fractional<RhsNumerator, RhsDenominator> const& rhs)
    -> decltype(!(lhs==rhs))
    {
        return !(lhs==rhs);
    }
}

#endif  // CNL_FRACTIONAL_H
