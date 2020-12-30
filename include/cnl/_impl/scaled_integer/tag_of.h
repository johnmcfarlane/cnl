
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_TAG_OF_H)
#define CNL_IMPL_SCALED_INTEGER_TAG_OF_H

#include "../num_traits/tag_of.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Scale>
    struct tag_of<scaled_integer<Rep, Scale>> : std::type_identity<Scale> {
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_TAG_OF_H
