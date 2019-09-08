
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
            : _base(convert<Scale, Rep>(f))
    {
    }

    template<typename Rep, class Scale>
    template<typename Numerator, typename Denominator>
    CNL_RELAXED_CONSTEXPR scaled_integer<Rep, Scale>&
    scaled_integer<Rep, Scale>::operator=(fraction<Numerator, Denominator> const& f)
    {
        _base::operator=(convert<Scale, Rep>(f));
        return *this;
    }

#if defined(__cpp_deduction_guides)
    template<typename Numerator, typename Denominator>
    scaled_integer(fraction<Numerator, Denominator>)
    -> scaled_integer<
            typename _impl::quotient_result<Numerator, Denominator>::type::rep,
            typename _impl::quotient_result<Numerator, Denominator>::type::scale>;
#endif
}

#endif  // CNL_IMPL_SCALED_INTEGER_FRACTION_H
