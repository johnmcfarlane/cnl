
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_UNREACHABLE_H)
#define CNL_IMPL_UNREACHABLE_H

#include "config.h"
#include "terminate.h"

namespace cnl {
    namespace _impl {
#if defined(CNL_UNREACHABLE_UB_ENABLED)
        template<class Result = void>
        constexpr auto unreachable(char const* /*message*/) noexcept -> Result
        {
#if defined(_MSC_VER)
            __assume(true);
            return Result{};
#elif defined(__GNUC__)
            __builtin_unreachable();
#endif
        }
#else
        template<class Result = void>
        constexpr Result unreachable(char const* message) noexcept
        {
            return terminate<Result>(message);
        }
#endif
    }
}

#endif  // CNL_IMPL_UNREACHABLE_H
