
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_NATIVE_H)
#define CNL_IMPL_OVERFLOW_NATIVE_H

#include "../native_tag.h"

/// compositional numeric library
namespace cnl {
    // match the behavior of fundamental arithmetic types
    struct native_overflow_tag : _impl::native_tag {
    };

    static constexpr native_overflow_tag native_overflow{};

    ////////////////////////////////////////////////////////////////////////////////
    // conversion overflow

    namespace _impl {
        template<typename Result>
        struct convert<native_overflow_tag, Result>
                : convert<_impl::native_tag, Result> {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // arithmetic overflow

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<native_overflow_tag, Operator>
                : tagged_binary_operator<_impl::native_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_NATIVE_H
