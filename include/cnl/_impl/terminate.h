
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_TERMINATE_H)
#define CNL_TERMINATE_H

#include "config.h"

#include <cstdio>
#include <exception>

namespace cnl {
    namespace _impl {
        template<class Result>
        [[noreturn]] constexpr Result terminate(char const* message) noexcept
        {
            std::fputs(message, stderr);
            std::fputc('\n', stderr);
            std::terminate();
        }
    }
}

#endif //CNL_TERMINATE_H
