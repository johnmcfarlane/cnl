
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H)
#define CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H

#include "../common.h"
#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"
#include "forward_declaration.h"
#include "type.h"

#include <type_traits>

namespace std {
    template<int Digits1, typename Narrowest1, int Digits2, typename Narrowest2>
    struct common_type<cnl::_impl::wide_integer<Digits1, Narrowest1>, cnl::_impl::wide_integer<Digits2, Narrowest2>> {
        static constexpr auto _max_digits = cnl::_impl::max(Digits1, Digits2);
        static constexpr auto _are_signed = cnl::is_signed<Narrowest1>::value || cnl::is_signed<Narrowest2>::value;
        using _common_type = typename std::common_type<Narrowest1, Narrowest2>::type;
        using _narrowest = cnl::_impl::set_signedness_t<_common_type, _are_signed>;

        using type = cnl::_impl::wide_integer<cnl::_impl::max(Digits1, Digits2), _narrowest>;
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_COMMON_TYPE_H
