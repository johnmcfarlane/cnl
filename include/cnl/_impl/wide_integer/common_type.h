
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H)
#define CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H

#include "../common.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
#include "type.h"

#include <type_traits>

namespace std {
    template<int Digits1, typename Narrowest1, int Digits2, typename Narrowest2>
    struct common_type<cnl::_impl::wide_integer<Digits1, Narrowest1>, cnl::_impl::wide_integer<Digits2, Narrowest2>> {
        static constexpr auto _max_digits = cnl::_impl::max(Digits1, Digits2);
        static constexpr auto _are_signed = cnl::is_signed<Narrowest1>::value || cnl::is_signed<Narrowest2>::value;
        using _common_type = typename std::common_type<Narrowest1, Narrowest2>::type;
        using _narrowest = cnl::_impl::set_signedness_t<_common_type, _are_signed>;

        static_assert(
                std::is_same<
                        typename std::common_type<
                                cnl::_impl::set_signedness_t<Narrowest1, _are_signed>,
                                cnl::_impl::set_signedness_t<Narrowest2, _are_signed>>::type,
                        _narrowest>::value,
                // Presumably, either Narrowest1 and/or Narrowest2 are innapropriate for use in wide_integer
                // or one or other way to calculate _narrowest is superior in some as-yet unforeseen way.
                "set_signedness<common_type> is not the same as common_type<set_signedness>");

        using type = cnl::_impl::wide_integer<cnl::_impl::max(Digits1, Digits2), _narrowest>;
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H
