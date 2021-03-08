
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_DECLARATION_H)
#define CNL_IMPL_WRAPPER_DECLARATION_H

#include "../../integer.h"
#include "../custom_operator/native_tag.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<integer Rep = int, tag Tag = native_tag>
        class wrapper;
    }
}

#endif  // CNL_IMPL_WRAPPER_DECLARATION_H
