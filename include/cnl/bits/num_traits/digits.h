
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/bits/num_traits/digits.h
/// \brief definitions supporting \ref cnl::set_digits

#ifndef CNL_BITS_NUM_TRAITS_DIGITS
#define CNL_BITS_NUM_TRAITS_DIGITS

#include "../../constant.h"
#include "../../limits.h"

#include "../used_digits.h"

#include <type_traits>

namespace cnl {
    template<typename T>
    struct digits : std::integral_constant<int, numeric_limits<T>::digits> {
        static_assert(numeric_limits<T>::is_specialized, "cnl::digits is not correctly specialized for T");
    };

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct digits<constant<Value>> : std::integral_constant<
            int,
            _impl::used_digits((Value<0) ? -Value : Value)> {
    };

#if (__cplusplus > 201402L)
    template<class T>
    constexpr int digits_v = digits<T>::value;
#endif
}

#endif  // CNL_BITS_NUM_TRAITS_DIGITS
