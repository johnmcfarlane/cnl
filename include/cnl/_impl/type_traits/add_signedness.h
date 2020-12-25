
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H)
#define CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H

#include "../num_traits/is_composite.h"
#include "../num_traits/rep.h"
#include "../num_traits/set_rep.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/type_identity.h"

#include <type_traits>

namespace cnl {
    /// \brief given a numeric type, defines member `type` as the equivalent signed type
    /// \tparam T type to transform into a signed type
    /// \headerfile cnl/num_traits.h
    /// \note User-specializations of this type are permitted.
    /// \sa cnl::remove_signedness, cnl::add_signedness_t, std::make_signed
    template<class T, class = void>
    struct add_signedness;

    template<class T>
    requires std::is_integral_v<T> struct add_signedness<T> : std::make_signed<T> {
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

    /// \brief helper alias of \ref cnl::add_signedness
    /// \headerfile cnl/num_traits.h
    /// \sa cnl::remove_signedness_t, cnl::add_signedness, std::make_signed_t
    template<class T>
    using add_signedness_t = typename add_signedness<T>::type;

    template<typename T>
    struct add_signedness<T, _impl::enable_if_t<is_composite<T>::value>>
        : _impl::type_identity<_impl::set_rep_t<T, add_signedness_t<_impl::rep_t<T>>>> {
    };
}

#endif  // CNL_IMPL_TYPE_TRAITS_ADD_SIGNEDNESS_H
