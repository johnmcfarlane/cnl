
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H)
#define CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H

#include "enable_if.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/from_rep.h"
#include "../type_traits/type_identity.h"
#include <type_traits>

namespace cnl {
    template<class, class = void>
    struct remove_signedness;

    template<class T>
    struct remove_signedness<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_unsigned<T> {
    };

#if defined(CNL_INT128_ENABLED)
    template<>
    struct remove_signedness<int128> {
        using type = uint128;
    };
    template<>
    struct remove_signedness<uint128> {
        using type = uint128;
    };
#endif

    template<class T>
    using remove_signedness_t = typename remove_signedness<T>::type;

    template<typename T>
    struct remove_signedness<T, _impl::enable_if_t<is_composite<T>::value>>
            : _impl::type_identity<_impl::from_rep_t<T, remove_signedness_t<_impl::to_rep_t<T>>>> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H
