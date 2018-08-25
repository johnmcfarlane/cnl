
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_REMOVE_CVREF_H)
#define CNL_IMPL_TYPE_TRAITS_REMOVE_CVREF_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::remove_cvref_t

        template<typename T>
        using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_REMOVE_CVREF_H
