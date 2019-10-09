
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_MAX_DIGITS)
#define CNL_IMPL_NUM_TRAITS_MAX_DIGITS

#include "../../cstdint.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/is_signed.h"
#include "digits.h"
#include "is_composite.h"
#include "to_rep.h"

namespace cnl {
    namespace _impl {
        // max_digits_fundamental
        template<bool Signed>
        struct max_digits_fundamental;

        template<>
        struct max_digits_fundamental<true> : digits<intmax> {
        };

        template<>
        struct max_digits_fundamental<false> : digits<uintmax> {
        };

        // max_digits
        template<typename T, class Enable = void>
        struct max_digits;

        template<typename T>
        struct max_digits<T, enable_if_t<is_integral<T>::value>>
                : max_digits_fundamental<is_signed<T>::value> {
        };

        template<typename T>
        struct max_digits<T, enable_if_t<is_composite<T>::value>>
                : max_digits<to_rep_t<T>> {
        };
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_MAX_DIGITS
