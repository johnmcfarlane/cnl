
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_COMMON_TYPE_H)
#define CNL_IMPL_TYPE_TRAITS_COMMON_TYPE_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // pre-C++14 common_type_t
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_COMMON_TYPE_H
