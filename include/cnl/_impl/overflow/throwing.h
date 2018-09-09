
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_THROWING_H)
#define CNL_IMPL_OVERFLOW_THROWING_H

#include "common.h"
#include "../throw_exception.h"

#include <stdexcept>

/// compositional numeric library
namespace cnl {
    // terminate program with diagnostic when overflow is detected
    // throw an exception when overflow is don overflowetected
    static constexpr struct throwing_overflow_tag {
    } throwing_overflow{};

    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, throwing_overflow_tag> {
            constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("positive overflow");
            }
        };

        template<typename Result>
        struct negative_overflow_result<Result, throwing_overflow_tag> {
            constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("negative overflow");
            }
        };
    }

    template<typename Result, typename Input>
    struct convert<throwing_overflow_tag, Result, Input>
            : _impl::overflow_convert<throwing_overflow_tag, Result, Input> {
    };

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<throwing_overflow_tag, Operator>
                : tagged_binary_overflow_operator<throwing_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_THROWING_H
