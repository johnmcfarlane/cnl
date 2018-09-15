
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_MAKE_WIDE_INTEGER_H)
#define CNL_IMPL_WIDE_INTEGER_MAKE_WIDE_INTEGER_H

#include "../num_traits/adopt_signedness.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Rep>
        constexpr auto make_wide_integer(Rep const& rep)
        -> wide_integer<
                digits<Rep>::value,
                adopt_signedness_t<int, Rep>> {
            return rep;
        }
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_MAKE_WIDE_INTEGER_H
