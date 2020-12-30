
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H)
#define CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H

#include "../polarity.h"

#include <type_traits>

namespace cnl {
    namespace _impl {
        template<
                int Value, typename Positive, typename Zero, typename Negative,
                polarity Polarity = measure_polarity(Value)>
        struct conditional3;

        template<int Value, typename Positive, typename Zero, typename Negative>
        struct conditional3<Value, Positive, Zero, Negative, polarity::neutral>
            : std::type_identity<Zero> {
        };

        template<int Value, typename Positive, typename Zero, typename Negative>
        struct conditional3<Value, Positive, Zero, Negative, polarity::negative>
            : std::type_identity<Negative> {
        };

        template<int Value, typename Positive, typename Zero, typename Negative>
        struct conditional3<Value, Positive, Zero, Negative, polarity::positive>
            : std::type_identity<Positive> {
        };

        template<int Value, typename Positive, typename Zero, typename Negative>
        using conditional3_t = typename conditional3<Value, Positive, Zero, Negative>::type;
    }
}

#endif  // CNL_IMPL_TYPE_TRAITS_CONDITIONAL3_H
