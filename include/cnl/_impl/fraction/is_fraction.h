
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_FRACTION_IS_FRACTION_H)
#define CNL_IMPL_FRACTION_IS_FRACTION_H 1

#include "type.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename T>
        struct is_fraction : std::false_type {
        };

        template<typename Numerator, typename Denominator>
        struct is_fraction<fraction<Numerator, Denominator>> : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_FRACTION_IS_FRACTION_H
