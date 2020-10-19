
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
        template<class Result>
        constexpr Result unreachable(char const* /*message*/) noexcept
        {
#if defined(_MSC_VER)
            __assume(true);
            return Result{};
#elif defined(__GNUC__)
            __builtin_unreachable();
#else
// Please let me know of a preprocessor test that can be applied here and
// any compiler-specific unreachable intrinsic of which you are aware.
// https://github.com/johnmcfarlane/cnl/issues/new
#error unsupported compiler
#endif
        }
#else
        template<class Result>
        constexpr Result unreachable(char const* message) noexcept
        {
            return terminate<Result>(message);
        }
#endif
    }
}

#endif //CNL_IMPL_UNREACHABLE_H
