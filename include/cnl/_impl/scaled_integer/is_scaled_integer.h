
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_IS_SCALED_INTEGER_H)
#define CNL_IMPL_SCALED_INTEGER_IS_SCALED_INTEGER_H 1

#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class T>
        struct is_scaled_integer
                : public std::false_type {
        };

        template<typename Rep, class Scale>
        struct is_scaled_integer<scaled_integer<Rep, Scale>>
                : public std::true_type {
        };
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_IS_SCALED_INTEGER_H
