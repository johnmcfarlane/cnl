
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_IS_SIGNED
#define CNL_IMPL_TYPE_TRAITS_IS_SIGNED

#include "../../cstdint.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/to_rep.h"
#include "enable_if.h"
#include "../../constant.h"

#include <type_traits>

namespace cnl {
    template<typename T, class Enable = void>
    struct is_signed;

    template<>
    struct is_signed<char> : std::is_signed<char> {
    };
    template<>
    struct is_signed<signed char> : std::true_type {
    };
    template<>
    struct is_signed<unsigned char> : std::false_type {
    };

    template<>
    struct is_signed<wchar_t> : std::is_signed<char> {
    };
    template<>
    struct is_signed<char16_t> : std::is_signed<char16_t> {
    };
    template<>
    struct is_signed<char32_t> : std::is_signed<char32_t> {
    };

    template<>
    struct is_signed<signed short> : std::true_type {
    };
    template<>
    struct is_signed<unsigned short> : std::false_type {
    };

    template<>
    struct is_signed<signed int> : std::true_type {
    };
    template<>
    struct is_signed<unsigned int> : std::false_type {
    };

    template<>
    struct is_signed<signed long> : std::true_type {
    };
    template<>
    struct is_signed<unsigned long> : std::false_type {
    };

    template<>
    struct is_signed<signed long long> : std::true_type {
    };
    template<>
    struct is_signed<unsigned long long> : std::false_type {
    };

#if defined(CNL_INT128_ENABLED)
    template<>
    struct is_signed<int128> : std::true_type {
    };
    template<>
    struct is_signed<uint128> : std::false_type {
    };
#endif

    template<>
    struct is_signed<float> : std::true_type {
    };
    template<>
    struct is_signed<double> : std::true_type {
    };
    template<>
    struct is_signed<long double> : std::true_type {
    };

    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct is_signed<constant<Value>> : is_signed<decltype(Value)> {
    };

    template<typename T>
    struct is_signed<T, _impl::enable_if_t<is_composite<T>::value>>
            : is_signed<_impl::to_rep_t<T>> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_SIGNED
