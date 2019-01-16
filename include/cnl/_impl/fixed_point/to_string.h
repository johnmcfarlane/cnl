
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_FIXED_POINT_TO_STRING_H
#define CNL_IMPL_FIXED_POINT_TO_STRING_H

#include "type.h"
#include "to_chars.h"

#include <string>

/// compositional numeric library
namespace cnl {
    using std::to_string;

    template<typename Rep, int Exponent>
    std::string to_string(cnl::fixed_point<Rep, Exponent> const& value)
    {
        auto chars = to_chars(value);
        return chars.data();
    }
}

#endif  // CNL_IMPL_FIXED_POINT_TO_STRING_H
