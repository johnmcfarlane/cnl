
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_IS_SIGNED)
#define CNL_IMPL_TYPE_TRAITS_IS_SIGNED

#include "../config.h"
#include "../cstdint/types.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/rep_of.h"
#include "enable_if.h"

#include <type_traits>

namespace cnl {
    template<typename T>
    inline constexpr bool is_signed = false;

    template<>
    inline constexpr bool is_signed<char> = std::is_signed_v<char>;
    template<>
    inline constexpr bool is_signed<signed char> = true;
    template<>
    inline constexpr bool is_signed<unsigned char> = false;

    template<>
    inline constexpr bool is_signed<wchar_t> = std::is_signed_v<char>;
    template<>
    inline constexpr bool is_signed<char16_t> = std::is_signed_v<char16_t>;
    template<>
    inline constexpr bool is_signed<char32_t> = std::is_signed_v<char32_t>;

    template<>
    inline constexpr bool is_signed<signed short> = true;
    template<>
    inline constexpr bool is_signed<unsigned short> = false;

    template<>
    inline constexpr bool is_signed<signed int> = true;
    template<>
    inline constexpr bool is_signed<unsigned int> = false;

    template<>
    inline constexpr bool is_signed<signed long> = true;
    template<>
    inline constexpr bool is_signed<unsigned long> = false;

    template<>
    inline constexpr bool is_signed<signed long long> = true;
    template<>
    inline constexpr bool is_signed<unsigned long long> = false;

#if defined(CNL_INT128_ENABLED)
    template<>
    inline constexpr bool is_signed<int128> = true;
    template<>
    inline constexpr bool is_signed<uint128> = false;
#endif

    template<>
    inline constexpr bool is_signed<float> = true;
    template<>
    inline constexpr bool is_signed<double> = true;
    template<>
    inline constexpr bool is_signed<long double> = true;

    template<typename T>
    requires is_composite<T>::value
    inline constexpr bool is_signed<T> = is_signed<_impl::rep_of_t<T>>;
}

#endif  // CNL_IMPL_TYPE_TRAITS_IS_SIGNED
