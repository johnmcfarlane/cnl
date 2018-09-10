
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_WRAP_H
#define CNL_IMPL_NUM_TRAITS_WRAP_H

#include "../type_traits/enable_if.h"
#include "is_composite.h"
#include "to_rep.h"

namespace cnl {
    namespace _impl {
        template<typename Number, typename Rep, typename Enable = void>
        struct wrap;

        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<!is_composite<Number>::value>> {
            constexpr Number operator()(Rep const& number) const
            {
                return number;
            }
        };

        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<is_composite<Number>::value>> {
            constexpr auto operator()(Rep const& rep) const
            -> decltype(from_rep<Number>(wrap<to_rep_t<Number>, Rep>{}(rep)))
            {
                return from_rep<Number>(wrap<to_rep_t<Number>, Rep>{}(rep));
            }
        };
    }

    template<typename Number, typename Rep>
    constexpr auto wrap(Rep const& rep)
    -> decltype(_impl::wrap<Number, Rep>{}(rep))
    {
        return _impl::wrap<Number, Rep>{}(rep);
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_WRAP_H
