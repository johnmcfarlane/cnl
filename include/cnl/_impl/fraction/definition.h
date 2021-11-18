
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_DEFINITION_H)
#define CNL_IMPL_FRACTION_DEFINITION_H

#include "../../fixed_point.h"
#include "../../integer.h"
#include "../num_traits/set_width.h"
#include "../ssizeof.h"

#include <concepts>
#include <limits>
#include <type_traits>

/// compositional numeric library
namespace cnl {

    /// \brief numeric type represented as the fraction, \ref numerator `/` \ref denominator
    ///
    /// \tparam Numerator the type of numerator
    /// \tparam Exponent the type of denominator

    template<fixed_point Numerator = int, fixed_point Denominator = Numerator>
    struct fraction {
        static_assert(
                std::numeric_limits<Numerator>::is_iec559 == std::numeric_limits<Denominator>::is_iec559,
                "ill-formed if only one template parameter is floating-point");

        /// alias to `Numerator`
        using numerator_type = Numerator;

        /// alias to `Denominator`
        using denominator_type = Denominator;

        explicit constexpr fraction(Numerator n, Denominator d);

        template<integer Integer>
        explicit constexpr fraction(Integer const& i);

        template<fixed_point RhsNumerator, fixed_point RhsDenominator>
        // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
        constexpr fraction(fraction<RhsNumerator, RhsDenominator> const& f);

        template<std::floating_point FloatingPoint>
        explicit constexpr fraction(FloatingPoint);

        /// returns the quotient, \ref numerator `/` \ref denominator
        template<std::floating_point Scalar>
        [[nodiscard]] explicit constexpr operator Scalar() const
        {
            return static_cast<Scalar>(numerator) / static_cast<Scalar>(denominator);
        }

        /// the numerator (top number) of the fraction
        numerator_type numerator;  // NOLINT(misc-non-private-member-variables-in-classes)

        /// the denominator (bottom number) of the fraction
        denominator_type denominator = 1;  // NOLINT(misc-non-private-member-variables-in-classes)
    };

    fraction(float)->fraction<_impl::set_width_t<int, _impl::ssizeof<float>() * CHAR_BIT>>;

    fraction(double)->fraction<_impl::set_width_t<int, _impl::ssizeof<double>() * CHAR_BIT>>;

#if defined(CNL_INT128_ENABLED)
    fraction(long double)->fraction<_impl::set_width_t<int, _impl::ssizeof<long double>() * CHAR_BIT>>;
#endif

    template<integer Integer>
    fraction(Integer) -> fraction<Integer>;
}

#endif  // CNL_IMPL_FRACTION_DEFINITION_H
