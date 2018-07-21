
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_BITS_TYPE_TRAITS_MAKE_UNSIGNED
#define CNL_BITS_TYPE_TRAITS_MAKE_UNSIGNED

#include <cnl/bits/type_traits.h>

namespace cnl {
    template<class, class = void>
    struct make_unsigned;

    template<class T>
    struct make_unsigned<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> {
    };

#if defined(CNL_INT128_ENABLED)
    template<>
    struct make_unsigned<int128> {
        using type = uint128;
    };
    template<>
    struct make_unsigned<uint128> {
        using type = uint128;
    };
#endif

    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;
}

#endif  // CNL_BITS_TYPE_TRAITS_MAKE_UNSIGNED
