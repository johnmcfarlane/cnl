
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_COMMON_H)
#define CNL_IMPL_OVERFLOW_COMMON_H

#include "builtin_overflow.h"
#include "is_overflow.h"
#include "overflow_operator.h"
#include "../native_tag.h"
#include "../polarity.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // result

        template<typename Result, class OverflowTag>
        struct positive_overflow_result;

        template<typename Result, class OverflowTag>
        struct negative_overflow_result;

        ////////////////////////////////////////////////////////////////////////////////
        // overflow_convert

        template<class OverflowTag, typename Destination, typename Source>
        struct convert_overflow_operator {
            CNL_NODISCARD constexpr Destination operator()(Source const& from) const
            {
                return is_overflow<convert_op, polarity::positive>{}.template operator()<Destination>(from)
                        ? overflow_operator<convert_op, OverflowTag, polarity::positive>{}.template operator()<
                                Destination>(from)
                        : is_overflow<convert_op, polarity::negative>{}.template operator()<Destination>(from)
                                ? overflow_operator<convert_op, OverflowTag, polarity::negative>{}.template operator()<
                                        Destination>(from)
                                : static_cast<Destination>(from);
            }
        };

        template<class OverflowTag, class Operator>
        struct unary_overflow_operator {
            template<typename Operand>
            CNL_NODISCARD constexpr auto operator()(Operand const& operand) const
            -> op_result<Operator, Operand>
            {
                return is_overflow<Operator, polarity::positive>{}(operand)
                        ? overflow_operator<Operator, OverflowTag, polarity::positive>{}(operand)
                        : is_overflow<Operator, polarity::negative>{}(operand)
                                ? overflow_operator<Operator, OverflowTag, polarity::negative>{}(operand)
                                : Operator{}(operand);
            }
        };

        template<class OverflowTag, class Operator>
        struct binary_overflow_operator {
#if defined(CNL_BUILTIN_OVERFLOW_ENABLED)
            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> enable_if_t<builtin_overflow_supported<Operator, Lhs, Rhs>(), op_result<Operator, Lhs, Rhs>>
            {
                return builtin_binary_overflow_operator<OverflowTag, Operator>(lhs, rhs);
            }
#endif

            template<class Lhs, class Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> enable_if_t<!builtin_overflow_supported<Operator, Lhs, Rhs>(), op_result<Operator, Lhs, Rhs>>
            {
                return is_overflow<Operator, polarity::positive>{}(lhs, rhs)
                        ? overflow_operator<Operator, OverflowTag, polarity::positive>{}(lhs, rhs)
                        : is_overflow<Operator, polarity::negative>{}(lhs, rhs)
                                ? overflow_operator<Operator, OverflowTag, polarity::negative>{}(lhs, rhs)
                                : Operator{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_COMMON_H
