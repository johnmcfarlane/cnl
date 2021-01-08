
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_CTORS_H)
#define CNL_IMPL_FRACTION_CTORS_H

#include "make_fraction.h"

#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Numerator, typename Denominator>
    constexpr fraction<Numerator, Denominator>::fraction(Numerator n, Denominator d)
        : numerator{std::move(n)}
        , denominator{std::move(d)}
    {
    }

    template<typename Numerator, typename Denominator>
    template<_impl::integer Integer>
    constexpr fraction<Numerator, Denominator>::fraction(Integer const& i)
        : fraction(static_cast<Numerator>(i), 1)
    {
    }

    template<typename Numerator, typename Denominator>
    template<typename RhsNumerator, typename RhsDenominator>
    constexpr fraction<Numerator, Denominator>::fraction(
            fraction<RhsNumerator, RhsDenominator> const& f)
        : fraction(static_cast<Numerator>(f.numerator), static_cast<Numerator>(f.denominator))
    {
    }

    template<typename Numerator, typename Denominator>
    template<_impl::floating_point FloatingPoint>
    constexpr fraction<Numerator, Denominator>::fraction(FloatingPoint d)
        : fraction(_impl::make_fraction<Numerator, Denominator>(d))
    {
    }
}

#endif  // CNL_IMPL_FRACTION_CTORS_H
