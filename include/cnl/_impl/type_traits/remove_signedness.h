
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H)
#define CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H

#include "../../limits.h"
#include "../num_traits/is_composite.h"
#include "../num_traits/rep_of.h"
#include "../num_traits/set_rep.h"
#include "enable_if.h"

#include <type_traits>

namespace cnl {
    /// \brief given a numeric type, defines member `type` as the equivalent unsigned type
    /// \tparam T type to transform into an unsigned type
    /// \headerfile cnl/num_traits.h
    /// \note User-specializations of this type are permitted.
    /// \sa cnl::add_signedness, cnl::remove_signedness_t, std::make_unsigned
    template<class T, class = void>
    struct remove_signedness;

    template<class T>
    requires std::is_integral_v<T> struct remove_signedness<T>
        : std::make_unsigned<T> {
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

    /// \brief helper alias of \ref cnl::remove_signedness
    /// \headerfile cnl/num_traits.h
    /// \sa cnl::add_signedness_t, cnl::remove_signedness, std::make_unsigned_t
    template<class T>
    using remove_signedness_t = typename remove_signedness<T>::type;

    template<typename T>
    struct remove_signedness<T, _impl::enable_if_t<is_composite<T>::value>>
        : std::type_identity<_impl::set_rep_t<T, remove_signedness_t<_impl::rep_of_t<T>>>> {
    };

    namespace _impl {
        template<typename T>
        constexpr auto as_unsigned(T const& t)
        {
            return remove_signedness_t<T>(t);
        }
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_REMOVE_SIGNEDNESS_H
