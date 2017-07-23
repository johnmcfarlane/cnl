
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief trait definitions related to the `cnl::fixed_point` type

#if !defined(SG14_TYPE_TRAITS_H)
#define SG14_TYPE_TRAITS_H 1

#include <type_traits>

/// study group 14 of the C++ working group
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
        constexpr bool identical(const A& a, const B& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
    }
}

#endif	// SG14_TYPE_TRAITS_H
