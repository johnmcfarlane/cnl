
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_FIXED_POINT_TO_STRING_H
#define CNL_IMPL_FIXED_POINT_TO_STRING_H

#include "type.h"
#include "to_chars.h"

#include <cassert>
#include <string>

/// compositional numeric library
namespace cnl {
    using std::to_string;

    template<typename Rep, int Exponent>
    std::string to_string(cnl::fixed_point<Rep, Exponent> const& value)
    {
        constexpr auto num_chars = _impl::max_decimal_digits<cnl::fixed_point<Rep, Exponent>>::value;
        char chars[num_chars+1];

        auto result = cnl::to_chars(chars, chars+num_chars, value);
        assert(result.ptr>std::begin(chars));
        assert(result.ptr<std::end(chars));
        assert(result.ec==std::errc{});

        *result.ptr = '\0';
        return std::begin(chars);
    }
}

#endif  // CNL_IMPL_FIXED_POINT_TO_STRING_H
