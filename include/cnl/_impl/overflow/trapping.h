
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_TRAPPING_H)
#define CNL_IMPL_OVERFLOW_TRAPPING_H

#include "common.h"
#include "overflow_operator.h"
#include "../terminate.h"

/// compositional numeric library
namespace cnl {
    // terminate program with diagnostic when overflow is detected
    static constexpr struct trapping_overflow_tag {
    } trapping_overflow{};

    namespace _impl {
        template<typename Operator>
        struct overflow_operator<Operator, trapping_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return terminate<Destination>("positive overflow");
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const&...) const
            {
                return terminate<op_result<Operator, Operands...>>("positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, trapping_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            constexpr Destination operator()(Source const&) const
            {
                return terminate<Destination>("negative overflow");
            }

            template<class ... Operands>
            constexpr op_result<Operator, Operands...> operator()(Operands const&...) const
            {
                return terminate<op_result<Operator, Operands...>>("negative overflow");
            }
        };
    }

    template<typename Result, typename Input>
    struct convert<trapping_overflow_tag, Result, Input>
            : _impl::overflow_convert<trapping_overflow_tag, Result, Input> {
    };

    namespace _impl {
        template<class Operator>
        struct tagged_binary_operator<trapping_overflow_tag, Operator>
                : tagged_binary_overflow_operator<trapping_overflow_tag, Operator> {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_TRAPPING_H
