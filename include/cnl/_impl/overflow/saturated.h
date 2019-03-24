
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_SATURATED_H)
#define CNL_IMPL_OVERFLOW_SATURATED_H

#include "common.h"
#include "overflow_operator.h"
#include "../terminate.h"

/// compositional numeric library
namespace cnl {
    // confine range of results
    static constexpr struct saturated_overflow_tag {
    } saturated_overflow{};

    namespace _impl {
        template<typename Operator>
        struct overflow_operator<Operator, saturated_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return numeric_limits<Destination>::max();
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return numeric_limits<op_result<Operator, Operands...>>::max();
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, saturated_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return numeric_limits<Destination>::lowest();
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return numeric_limits<op_result<Operator, Operands...>>::lowest();
            }
        };
    }

    template<typename Result, typename Input>
    struct convert<saturated_overflow_tag, Result, Input>
            : _impl::overflow_convert<saturated_overflow_tag, Result, Input> {
    };

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<saturated_overflow_tag, Operator>
                : tagged_binary_overflow_operator<saturated_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_SATURATED_H
