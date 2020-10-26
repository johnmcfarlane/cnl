
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief contains fixed_point alias to scaled_integer

#if !defined(CNL_FIXED_POINT_H)
#define CNL_FIXED_POINT_H

#include "scaled_integer.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep = int, int Exponent = 0, int Radix = 2>
    using fixed_point = scaled_integer<Rep, power<Exponent, Radix>>;

    template<typename Value>
    constexpr auto make_fixed_point(Value const& value)
    {
        return make_scaled_integer(value);
    }
}

#endif  // CNL_FIXED_POINT_H
