
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_OPERATOR_H)
#define CNL_IMPL_OVERFLOW_OPERATOR_H

#include "polarity.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Operator, typename OverflowTag, polarity Polarity>
        struct overflow_operator;
    }
}

#endif  // CNL_IMPL_OVERFLOW_OPERATOR_H
