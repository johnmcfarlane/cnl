
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_COMMON_H)
#define CNL_IMPL_OVERFLOW_COMMON_H

#include "is_overflow.h"
#include "overflow_operator.h"
#include "polarity.h"
#include "../native_tag.h"

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
        struct overflow_convert {
            constexpr Destination operator()(Source const& from) const
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
        struct tagged_binary_overflow_operator {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> op_result<Operator, Lhs, Rhs>
            {
                using overflow_test = overflow_test<Operator, Lhs, Rhs>;
                return overflow_test::positive(lhs, rhs)
                        ? overflow_operator<Operator, OverflowTag, polarity::positive>{}(lhs, rhs)
                        : overflow_test::negative(lhs, rhs)
                                ? overflow_operator<Operator, OverflowTag, polarity::negative>{}(lhs, rhs)
                                : tagged_binary_operator<native_tag, Operator>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_COMMON_H
