
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential `fraction` definitions related to the `fixed_point` type

#if !defined(CNL_IMPL_FIXED_POINT_FRACTION_H)
#define CNL_IMPL_FIXED_POINT_FRACTION_H 1

#include "named.h"

#include "../fraction/type.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
    constexpr fixed_point<Rep, Exponent, Radix>::fixed_point(fraction<Numerator, Denominator> const& f)
            : fixed_point(quotient<fixed_point>(f.numerator, f.denominator))
    {
    }

    template<typename Rep, int Exponent, int Radix>
    template<typename Numerator, typename Denominator>
    CNL_RELAXED_CONSTEXPR fixed_point<Rep, Exponent, Radix>&
    fixed_point<Rep, Exponent, Radix>::operator=(fraction<Numerator, Denominator> const& f)
    {
        return operator=(quotient<fixed_point>(f.numerator, f.denominator));
    }

#if defined(__cpp_deduction_guides)
    template<typename Numerator, typename Denominator>
    fixed_point(fraction<Numerator, Denominator>)
    -> fixed_point<
            typename decltype(quotient(std::declval<Numerator>(), std::declval<Denominator>()))::rep,
            decltype(quotient(std::declval<Numerator>(), std::declval<Denominator>()))::exponent>;
#endif
}

#endif  // CNL_IMPL_FIXED_POINT_FRACTION_H
