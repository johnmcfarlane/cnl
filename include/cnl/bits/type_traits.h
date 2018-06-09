
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `cnl::fixed_point` type

#if !defined(CNL_TYPE_TRAITS_H)
#define CNL_TYPE_TRAITS_H 1

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::common_type_t

        // pre-C++14 common_type_t
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::enable_if_t

        // pre-C++14 enable_if_t
        template<bool C, class ... T>
        using enable_if_t = typename std::enable_if<C, T ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::identical - compiles iff same type; returns true iff equal

        template<class A, class B>
        constexpr bool identical(A const& a, B const& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::remove_cvref_t

        template<typename T>
        using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    }
}

#endif  // CNL_TYPE_TRAITS_H
