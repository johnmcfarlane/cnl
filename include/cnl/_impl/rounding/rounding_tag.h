
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_ROUNDING_TAG_H

#include "../native_tag.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // do what the rep type would do
        struct native_rounding_tag : public native_tag {
        };

        // a reasonable default
        struct nearest_rounding_tag {
        };
    }
}

#endif  // CNL_IMPL_ROUNDING_ROUNDING_TAG_H
