
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_MAKE_NUMBER_H)
#define CNL_IMPL_NUMBER_MAKE_NUMBER_H

#include "definition.h"
#include "../operators/native_tag.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Tag=native_tag, typename Rep=int>
        CNL_NODISCARD constexpr auto make_number(Rep const& rep)
        -> number<Rep, Tag> {
            return rep;
        }
    }
}

#endif  // CNL_IMPL_NUMBER_MAKE_NUMBER_H
