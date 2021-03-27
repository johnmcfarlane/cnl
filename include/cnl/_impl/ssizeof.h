
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SSIZEOF_H)
#define CNL_IMPL_SSIZEOF_H

#include "narrow_cast.h"

namespace cnl::_impl {
    template<typename T>
    [[nodiscard]] inline constexpr auto ssizeof() noexcept
    {
        return narrow_cast<int>(sizeof(T));
    }

    template<typename T>
    [[nodiscard]] inline constexpr auto ssizeof(T const&) noexcept
    {
        return narrow_cast<int>(sizeof(T));
    }
}

#endif  // CNL_IMPL_SSIZEOF_H
