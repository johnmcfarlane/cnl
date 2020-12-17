
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_DECLARATION_H)
#define CNL_IMPL_NUMBER_DECLARATION_H

#include "../operators/native_tag.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep = int, class Tag = native_tag>
        class number;
    }
}

#endif // CNL_IMPL_NUMBER_DECLARATION_H
