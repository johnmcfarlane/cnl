
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FIXED_POINT_IS_FIXED_POINT_H)
#define CNL_IMPL_FIXED_POINT_IS_FIXED_POINT_H 1

#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };

        template<typename Rep, int Exponent, int Radix>
        struct is_fixed_point<fixed_point<Rep, Exponent, Radix>>
                : public std::true_type {
        };
    }
}

#endif  // CNL_IMPL_FIXED_POINT_IS_FIXED_POINT_H
