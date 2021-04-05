
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_OSTREAM_H)
#define CNL_IMPL_WRAPPER_OSTREAM_H

#include "../custom_operator/tag.h"
#include "../ostream.h"
#include "is_wrapper.h"
#include "to_rep.h"

#if defined(CNL_IOSTREAMS_ENABLED)
#include <ostream>
#endif

/// compositional numeric library
namespace cnl {
    namespace _impl {
#if defined(CNL_IOSTREAMS_ENABLED)
        template<any_wrapper N>
        auto& operator<<(std::ostream& o, N const& i)
        {
            return o << to_rep(i);
        }
#endif
    }
}

#endif  // CNL_IMPL_WRAPPER_OSTREAM_H
