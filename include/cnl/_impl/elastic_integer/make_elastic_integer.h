
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_MAKE_ELASTIC_INTEGER_H)
#define CNL_IMPL_ELASTIC_INTEGER_MAKE_ELASTIC_INTEGER_H

#include "../../constant.h"
#include "../type_traits/enable_if.h"
#include "definition.h"
#include "digits.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    CNL_NODISCARD constexpr auto make_elastic_integer(constant<Value>)
            -> elastic_integer<digits<constant<Value>>::value>
    {
        return elastic_integer<digits<constant<Value>>::value>{Value};
    }

    namespace _impl {
        template<class Narrowest, class Integral>
        struct make_narrowest {
            using type = Narrowest;
        };

        template<class Integral>
        struct make_narrowest<void, Integral> {
            using type = narrowest<Integral>;
        };

        template<class Narrowest, class Integral>
        using make_narrowest_t = typename make_narrowest<Narrowest, Integral>::type;

        template<class Narrowest, class Integral>
        using make_type =
                elastic_integer<digits<Integral>::value, make_narrowest_t<Narrowest, Integral>>;
    }

    template<
            class Narrowest = void, class Integral,
            _impl::enable_if_t<!_impl::is_constant<Integral>::value, int> = 0>
    CNL_NODISCARD constexpr auto make_elastic_integer(Integral const& value)
            -> _impl::make_type<Narrowest, Integral>
    {
        return _impl::make_type<Narrowest, Integral>{value};
    }
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_MAKE_ELASTIC_INTEGER_H
