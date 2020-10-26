
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_WRAP_H)
#define CNL_IMPL_NUM_TRAITS_WRAP_H

#include "../type_traits/enable_if.h"
#include "is_composite.h"
#include "rep.h"

namespace cnl {
    namespace _impl {
        template<typename Number, typename Rep, typename Enable = void>
        struct wrap;

        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<!is_composite<Number>::value>> {
            CNL_NODISCARD constexpr Number operator()(Rep const& number) const
            {
                return number;
            }
        };

        template<typename Number, typename Rep>
        struct wrap<Number, Rep, enable_if_t<is_composite<Number>::value>> {
            CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
            {
                return from_rep<Number>(wrap<rep_t<Number>, Rep>{}(rep));
            }
        };
    }

    template<typename Number, typename Rep>
    CNL_NODISCARD constexpr auto wrap(Rep const& rep)
    {
        return _impl::wrap<Number, Rep>{}(rep);
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_WRAP_H
