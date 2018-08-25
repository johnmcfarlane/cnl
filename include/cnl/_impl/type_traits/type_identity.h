
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_TYPE_IDENTITY_H)
#define CNL_IMPL_TYPE_TRAITS_TYPE_IDENTITY_H

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::type_identity (wg21.link/p0887r1)

        template<typename T>
        struct type_identity {
            using type = T;
        };

        template<typename T>
        using type_identity_t = typename type_identity<T>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_TYPE_IDENTITY_H
