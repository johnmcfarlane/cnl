
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_THROW_EXCEPTION_H
#define CNL_THROW_EXCEPTION_H

#include "config.h"
#include "terminate.h"

#include <type_traits>

namespace cnl {
    namespace _impl {
        template<typename Result, class Exception>
        Result throw_exception(char const* message)
        {
#if defined(CNL_EXCEPTIONS_ENABLED)
            return true ? throw Exception(message) : Result{};
#else
            return terminate<Result>(message);
#endif
        }
    }
}

#endif //CNL_THROW_EXCEPTION_H
