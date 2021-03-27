
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NARROW_CAST_H)
#define CNL_IMPL_NARROW_CAST_H

#include "../number.h"
#include "cnl_assert.h"

namespace cnl::_impl {
    template<number To, number From>
    [[nodiscard]] inline constexpr auto narrow_cast(From const& y) noexcept
    {
        auto const x = static_cast<To>(y);
        CNL_ASSERT(static_cast<From>(x) == y);
        return x;
    }
}

#endif  // CNL_IMPL_NARROW_CAST_H
