
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
    template<typename Numerator, typename Denominator>
    scaled_integer(fraction<Numerator, Denominator>)
    -> scaled_integer<
            typename _impl::quotient_result<Numerator, Denominator>::rep,
            typename _impl::quotient_result<Numerator, Denominator>::scale>;
}

#endif  // CNL_IMPL_SCALED_INTEGER_FRACTION_H
