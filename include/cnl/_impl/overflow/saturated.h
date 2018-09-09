
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_SATURATED_H)
#define CNL_IMPL_OVERFLOW_SATURATED_H

#include "common.h"
#include "../terminate.h"

/// compositional numeric library
namespace cnl {
    // confine range of results
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};

    namespace _impl {
        template<typename Result>
        struct positive_overflow_result<Result, saturated_overflow_tag> {
            constexpr Result operator()() const
            {
                return numeric_limits<Result>::max();
            }
        };

        template<typename Result>
        struct negative_overflow_result<Result, saturated_overflow_tag> {
            constexpr Result operator()() const
            {
                return numeric_limits<Result>::lowest();
            }
        };
    }

    template<typename Result, typename Input>
    struct convert<saturated_overflow_tag, Result, Input>
            : _impl::overflow_convert<saturated_overflow_tag, Result, Input> {
    };

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<saturated_overflow_tag, Operator>
                : tagged_binary_overflow_operator<saturated_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_SATURATED_H
