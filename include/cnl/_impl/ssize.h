
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SSIZE_H)
#define CNL_IMPL_SSIZE_H

#include "narrow_cast.h"

namespace cnl::_impl {
    template<typename Container>
    [[nodiscard]] inline constexpr auto ssize(Container const& c) noexcept
    {
        return narrow_cast<int>(std::ssize(c));
    }
}

#endif  // CNL_IMPL_SSIZE_H
