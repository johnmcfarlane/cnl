
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_ENABLE_IF
#define CNL_IMPL_TYPE_TRAITS_ENABLE_IF

#include <type_traits>

namespace cnl {
    namespace _impl {
        // pre-C++14 enable_if_t
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_ENABLE_IF
