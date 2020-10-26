
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INC_DEC_OPERATOR_H)
#define CNL_IMPL_SCALED_INC_DEC_OPERATOR_H

#include "../operators/generic.h"
#include "../power_value.h"
#include "power.h"

/// compositional numeric library
namespace cnl {
    template<class Operator, int Exponent, int Radix, typename Rhs>
    struct pre_operator<Operator, power<Exponent, Radix>, Rhs> {
        constexpr auto operator()(Rhs& rhs) const
        {
            return typename _impl::pre_to_assign<Operator>::type{}(
                    rhs, _impl::power_value<Rhs, -Exponent, Radix>());
        }
    };

    template<typename Operator, int Exponent, int Radix, typename Lhs>
    struct post_operator<Operator, power<Exponent, Radix>, Lhs> {
        constexpr auto operator()(Lhs& lhs) const
        -> Lhs
        {
            auto copy = lhs;
            typename _impl::post_to_assign<Operator>::type{}(
                    lhs, _impl::power_value<Lhs, -Exponent, Radix>());
            return copy;
        }
    };
}

#endif // CNL_IMPL_SCALED_INC_DEC_OPERATOR_H
