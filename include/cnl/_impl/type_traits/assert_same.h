
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_ASSERT_SAME_H)
#define CNL_IMPL_TYPE_TRAITS_ASSERT_SAME_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        void assert_same(auto&& expected, auto&& actual)
        {
            static_assert(std::is_same_v<decltype(expected), decltype(actual)>);
        }
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_ASSERT_SAME_H
