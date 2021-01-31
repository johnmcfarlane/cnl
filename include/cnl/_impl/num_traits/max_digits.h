
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_MAX_DIGITS)
#define CNL_IMPL_NUM_TRAITS_MAX_DIGITS

#include "../cstdint/types.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/is_signed.h"
#include "digits.h"
#include "is_composite.h"
#include "rep_of.h"

namespace cnl {
    namespace _impl {
        // max_digits_fundamental
        template<bool Signed>
        inline constexpr int max_digits_fundamental = 0;

        template<>
        inline constexpr auto max_digits_fundamental<true> = digits<intmax>;

        template<>
        inline constexpr auto max_digits_fundamental<false> = digits<uintmax>;

        // max_digits
        template<typename T, class Enable = void>
        inline constexpr int max_digits = 0;

        template<_impl::integral T>
        inline constexpr auto max_digits<T> = max_digits_fundamental<is_signed<T>>;

        template<typename T>
        requires is_composite<T>::value inline constexpr auto max_digits<T> = max_digits<rep_of_t<T>>;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_MAX_DIGITS
