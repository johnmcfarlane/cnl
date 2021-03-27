
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
        // cnl::_impl::assert_same - instead of being false_type like std::is_same,
        // actually emits error which generally helps report the type of Actual

        template<typename Expected, typename Actual>
        struct assert_same {
        };

        template<typename T>
        struct assert_same<T, T> : std::true_type {
        };

        template<typename Expected, typename Actual>
        inline constexpr auto assert_same_v = assert_same<Expected, Actual>::value;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_ASSERT_SAME_H
