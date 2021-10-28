
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ABORT_H)
#define CNL_ABORT_H

#include "config.h"

#include <cstdio>
#include <cstdlib>

namespace cnl {
    namespace _impl {
        template<class Result>
        [[noreturn]] constexpr auto abort(char const* message) noexcept -> Result
        {
            std::fputs(message, stderr);
            std::fputc('\n', stderr);
            std::abort();
        }
    }
}

#endif  // CNL_ABORT_H
