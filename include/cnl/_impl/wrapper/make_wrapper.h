
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_MAKE_WRAPPER_H)
#define CNL_IMPL_WRAPPER_MAKE_WRAPPER_H

#include "../operators/native_tag.h"
#include "../operators/tag.h"
#include "definition.h"
#include "from_rep.h"
#include "integer.h"
#include "rep_of.h"
#include "tag_of.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<tag Tag = tag_of_t<wrapper<>>, typename Rep = rep_of_t<wrapper<>>>
        [[nodiscard]] constexpr auto make_wrapper(Rep const& rep) -> wrapper<Rep, Tag>
        {
            return wrapper<Rep, Tag>{rep};
        }
    }
}

#endif  // CNL_IMPL_WRAPPER_MAKE_WRAPPER_H
