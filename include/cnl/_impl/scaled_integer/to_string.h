
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TO_STRING_H)
#define CNL_IMPL_SCALED_INTEGER_TO_STRING_H

#include "../../integer.h"
#include "../narrow_cast.h"
#include "../scaled/is_scaled_tag.h"
#include "definition.h"
#include "to_chars.h"

#include <string>

/// compositional numeric library
namespace cnl {
    using std::to_string;

    template<integer Rep, scaled_tag Scale>
    auto to_string(cnl::scaled_integer<Rep, Scale> const& value)
    {
        auto const [chars, length] = to_chars_static(value);
        return std::string{chars.data(), _impl::narrow_cast<unsigned>(length)};
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_TO_STRING_H
