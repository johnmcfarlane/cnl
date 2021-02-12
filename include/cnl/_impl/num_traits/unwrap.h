
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_UNWRAP_H)
#define CNL_IMPL_NUM_TRAITS_UNWRAP_H

#include "../type_traits/enable_if.h"
#include "is_composite.h"
#include "rep_of.h"
#include "to_rep.h"

namespace cnl {
    namespace _impl {
        template<typename Number, typename Enable = void>
        struct unwrap;

        template<typename Number>
        struct unwrap<Number, enable_if_t<!is_composite<Number>::value>> {
            [[nodiscard]] constexpr auto operator()(Number const& number) const
            {
                return number;
            }
        };

        template<typename Number>
        struct unwrap<Number, enable_if_t<is_composite<Number>::value>> {
            [[nodiscard]] constexpr auto operator()(Number const& number) const
            {
                return unwrap<rep_of_t<Number>>{}(to_rep(number));
            }
        };
    }

    template<typename Number>
    [[nodiscard]] constexpr auto unwrap(Number const& number)
    {
        return _impl::unwrap<Number>{}(number);
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_UNWRAP_H
