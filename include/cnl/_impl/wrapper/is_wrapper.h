
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_IS_WRAPPER_H)
#define CNL_IMPL_WRAPPER_IS_WRAPPER_H

#include "../operators/tag.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename T>
        inline constexpr bool is_wrapper = false;

        template<typename Rep, tag Tag>
        inline constexpr auto is_wrapper<_impl::wrapper<Rep, Tag>> = true;

        template<typename T>
        concept any_wrapper = is_wrapper<T>;
    }
}

#endif  // CNL_IMPL_WRAPPER_IS_WRAPPER_H
