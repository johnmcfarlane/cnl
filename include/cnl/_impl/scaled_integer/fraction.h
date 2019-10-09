
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential `fraction` definitions related to the `scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_FRACTION_H)
#define CNL_IMPL_SCALED_INTEGER_FRACTION_H

#include "named.h"

#include "../../fraction.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Scale>
    template<typename Numerator, typename Denominator>
    constexpr scaled_integer<Rep, Scale>::scaled_integer(fraction<Numerator, Denominator> const& f)
            : scaled_integer(quotient<scaled_integer>(f.numerator, f.denominator))
    {
    }

    template<typename Rep, class Scale>
    template<typename Numerator, typename Denominator>
    CNL_RELAXED_CONSTEXPR scaled_integer<Rep, Scale>&
    scaled_integer<Rep, Scale>::operator=(fraction<Numerator, Denominator> const& f)
    {
        operator=(quotient<scaled_integer>(f.numerator, f.denominator));
        return *this;
    }

#if defined(__cpp_deduction_guides)
    template<typename Numerator, typename Denominator>
    scaled_integer(fraction<Numerator, Denominator>)
    -> scaled_integer<
            typename decltype(quotient(std::declval<Numerator>(), std::declval<Denominator>()))::rep,
            typename decltype(quotient(std::declval<Numerator>(), std::declval<Denominator>()))::scale>;
#endif
}

#endif  // CNL_IMPL_SCALED_INTEGER_FRACTION_H
