
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(CNL_COMMON_H)
#define CNL_COMMON_H 1

#include "config.h"

#define CNL_STR_HELPER(x) #x
#define CNL_STR(x) CNL_STR_HELPER(x)

// CNL_LIKELY - hints that a condition is likely to be true
#if defined(__clang__) || defined(__GNUC__)
#define CNL_LIKELY(CONDITION) __builtin_expect(!!(CONDITION), 1)
#define CNL_UNLIKELY(CONDITION) __builtin_expect(!!(CONDITION), 0)
#else
#define CNL_LIKELY(CONDITION) (!!(CONDITION))
#define CNL_UNLIKELY(CONDITION) (!!(CONDITION))
#endif

// CNL_ASSUME - hints that a condition *must* be true
#ifdef _MSC_VER
#define CNL_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__)
#define CNL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define CNL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

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
