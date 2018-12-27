
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H
#define CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H

#include "type_identity.h"

namespace cnl {
    namespace _impl {
        template<
                int Sign,
                typename Positive, typename Zero, typename Negative,
                bool IsNegative = (Sign<0), bool IsPositive = (Sign>0)>
        struct conditional3;

        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, false, false> : type_identity<Zero> {
        };

        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, true, false> : type_identity<Negative> {
        };

        template<int Sign, typename Positive, typename Zero, typename Negative>
        struct conditional3<Sign, Positive, Zero, Negative, false, true> : type_identity<Positive> {
        };

        template<int Sign, typename Positive, typename Zero, typename Negative>
        using conditional3_t = typename conditional3<Sign, Positive, Zero, Negative>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H
