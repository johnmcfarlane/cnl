
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(SG14_COMMON_H)
#define SG14_COMMON_H 1

namespace sg14 {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_type_t

        // pre-C++14 common_type_t
        // TODO: If this needs decay, so does common_type. Investigate. Remove decay here
        template<class ... T>
        using common_type_t = typename std::common_type<typename std::decay<T>::type ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::identical - compiles iff same type; returns true iff equal

        template<typename A, typename B>
        constexpr bool identical(const A& a, const B& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        };
    }
}

#endif  // SG14_COMMON_H
