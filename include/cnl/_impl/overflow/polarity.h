
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_POLARITY_H)
#define CNL_IMPL_OVERFLOW_POLARITY_H

/// compositional numeric library
namespace cnl {
    namespace _impl {
        enum class polarity {
            negative = -1,
            positive = 1
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_POLARITY_H
