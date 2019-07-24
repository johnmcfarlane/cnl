
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_MAKE_INTEGER_H)
#define CNL_IMPL_INTEGER_MAKE_INTEGER_H

#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class Tag, typename Rep>
        CNL_NODISCARD constexpr auto integer_from_rep(Rep const& rep)
        -> integer<Rep, Tag> {
            return from_rep<integer<Rep, Tag>>(rep);
        }

        template<class Tag=integer<>::tag, typename Rep=integer<>::rep>
        CNL_NODISCARD constexpr auto make_integer(Rep const& rep)
        -> integer<Rep, Tag> {
            return integer<Rep, Tag>{rep};
        }
    }
}

#endif  // CNL_IMPL_INTEGER_MAKE_INTEGER_H
