
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_UNDEFINED_H)
#define CNL_IMPL_OVERFLOW_UNDEFINED_H

#include "common.h"
#include "../unreachable.h"

/// compositional numeric library
namespace cnl {
    // terminate program with diagnostic when overflow is detected
    static constexpr struct undefined_overflow_tag {
    } undefined_overflow{};

    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, undefined_overflow_tag> {
            constexpr Result operator()() const
            {
                return unreachable<Result>("positive overflow");
            }
        };

        template<typename Result>
        struct negative_overflow_result<Result, undefined_overflow_tag> {
            constexpr Result operator()() const
            {
                return unreachable<Result>("negative overflow");
            }
        };
    }

    template<typename Result, typename Input>
    struct convert<undefined_overflow_tag, Result, Input>
            : _impl::overflow_convert<undefined_overflow_tag, Result, Input> {
    };

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<undefined_overflow_tag, Operator>
                : tagged_binary_overflow_operator<undefined_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_UNDEFINED_H
