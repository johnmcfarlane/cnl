
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TO_STRING_H)
#define CNL_IMPL_SCALED_INTEGER_TO_STRING_H

#include "to_chars.h"
#include "type.h"

#include <string>

/// compositional numeric library
namespace cnl {
    using std::to_string;

    template<typename Rep, int Exponent>
    std::string to_string(cnl::scaled_integer<Rep, power<Exponent>> const&
                                  value) // NOLINT(misc-unused-parameters)
    {
        auto chars = to_chars(value);
        return chars.data();
    }
}

#endif // CNL_IMPL_SCALED_INTEGER_TO_STRING_H
