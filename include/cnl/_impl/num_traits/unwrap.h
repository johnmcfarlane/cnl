
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_UNWRAP_H)
#define CNL_IMPL_NUM_TRAITS_UNWRAP_H

#include "is_composite.h"
#include "rep_of.h"
#include "to_rep.h"

namespace cnl {
    namespace _impl {
        template<typename Number>
        struct unwrap;

        template<typename Number>
        requires(!is_composite<Number>::value) struct unwrap<Number> {
            [[nodiscard]] constexpr auto operator()(Number const& number) const
            {
                return number;
            }
        };

        template<typename Number>
        requires is_composite_v<Number> struct unwrap<Number> {
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
