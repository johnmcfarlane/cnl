
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(CNL_COMMON_H)
#define CNL_COMMON_H 1

#define CNL_STR_HELPER(x) #x
#define CNL_STR(x) CNL_STR_HELPER(x)

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::min

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }
    }
}

#endif  // CNL_COMMON_H
