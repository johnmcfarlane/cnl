
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H
#define CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H

#include "../type_traits/enable_if.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/to_rep.h"
#include "../num_traits/from_rep.h"
#include "../type_traits/type_identity.h"
#include <type_traits>

namespace cnl {
    template<class, class = void>
    struct add_signedness;

    template<class T>
    struct add_signedness<T, _impl::enable_if_t<std::is_integral<T>::value>> : std::make_signed<T> {
    };

#if defined(CNL_INT128_ENABLED)
    template<>
    struct add_signedness<int128> {
        using type = int128;
    };
    template<>
    struct add_signedness<uint128> {
        using type = int128;
    };
#endif

    template<class T>
    using add_signedness_t = typename add_signedness<T>::type;

    template<typename T>
    struct add_signedness<T, _impl::enable_if_t<is_composite<T>::value>>
            : _impl::type_identity<_impl::from_rep_t<T, add_signedness_t<_impl::to_rep_t<T>>>> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H
