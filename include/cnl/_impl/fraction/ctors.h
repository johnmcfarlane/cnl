
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_CTORS_H)
#define CNL_IMPL_FRACTION_CTORS_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Numerator, typename Denominator>
    constexpr
    fraction<Numerator, Denominator>::fraction(Numerator const& n, Denominator const& d)
            : numerator{n}, denominator{d} {}

    template<typename Numerator, typename Denominator>
    constexpr
    fraction<Numerator, Denominator>::fraction(Numerator const& n)
            : fraction{n, 1} {}

    template<typename Numerator, typename Denominator>
    template<typename RhsNumerator, typename RhsDenominator>
    constexpr
    fraction<Numerator, Denominator>::fraction(fraction<RhsNumerator, RhsDenominator> const& f)
            : fraction{f.numerator, f.denominator} { }
}

#endif  // CNL_IMPL_FRACTION_CTORS_H
