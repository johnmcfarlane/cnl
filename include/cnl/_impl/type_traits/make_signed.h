
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_MAKE_SIGNED
#define CNL_IMPL_TYPE_TRAITS_MAKE_SIGNED

#include "../type_traits/enable_if.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/from_rep.h"
#include "../type_traits/type_identity.h"
#include <type_traits>

namespace cnl {
    template<class, class = void>
    struct make_signed;

    template<class T>
    struct make_signed<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };

#if defined(CNL_INT128_ENABLED)
    template<>
    struct make_signed<int128> {
        using type = int128;
    };
    template<>
    struct make_signed<uint128> {
        using type = int128;
    };
#endif

    template<class T>
    using make_signed_t = typename make_signed<T>::type;

    template<typename T>
    struct make_signed<T, _impl::enable_if_t<is_composite<T>::value>>
            : _impl::type_identity<_impl::from_rep_t<T, make_signed_t<_impl::to_rep_t<T>>>> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_MAKE_SIGNED
