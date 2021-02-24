
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_NUMBER_H)
#define CNL_IMPL_FRACTION_NUMBER_H

#include "../../number.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    template<typename Numerator, class Denominator>
    inline constexpr auto is_number_v<fraction<Numerator, Denominator>> = true;
}

#endif  // CNL_IMPL_FRACTION_NUMBER_H
