
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_IDENTICAL_H)
#define CNL_IMPL_TYPE_TRAITS_IDENTICAL_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::identical - compiles iff same type; returns true iff equal

        template<class A, class B>
        constexpr bool identical(A const& a, B const& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_IDENTICAL_H
