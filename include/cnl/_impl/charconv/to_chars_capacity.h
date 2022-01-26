
//          Copyright John McFarlane 2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H)
#define CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H

#include "../../integer.h"
#include "../cnl_assert.h"
#include "../numbers/signedness.h"

#include <limits>
#include <numbers>

namespace cnl::_impl {
    // maximum number of characters necessary to represent given Scalar
    // in give base in human-readable text excluding nul terminator,
    // e.g. to_chars_capacity<int8_t>{}() == 4  // ["-128".."127"]
    template<typename Scalar>
    struct to_chars_capacity;

    template<typename Scalar>
    requires integer<Scalar>
    struct to_chars_capacity<Scalar> {
        [[nodiscard]] constexpr auto operator()(int /*base*/ = 10) const
        {
            auto const sign_chars = static_cast<int>(cnl::numbers::signedness_v<Scalar>);
            auto const integer_chars = static_cast<int>(std::numeric_limits<Scalar>::digits * std::numbers::ln2 / std::numbers::ln10) + 1;
            return sign_chars + integer_chars;
        }
    };
}

#endif  // CNL_IMPL_CHARCONV_TO_CHARS_CAPACITY_H
