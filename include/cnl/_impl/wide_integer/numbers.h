
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_NUMBERS_H)
#define CNL_IMPL_WIDE_INTEGER_NUMBERS_H

#include "../../numbers.h"
#include "../num_traits/adopt_width.h"
#include "definition.h"

/// compositional numeric library, numbers header/namespace
namespace cnl::numbers {
    template<int Digits, class Narrowest>
    struct signedness<wide_integer<Digits, Narrowest>> : signedness<Narrowest> {
    };

    template<int Digits, class Narrowest, bool IsSigned>
    struct set_signedness<wide_integer<Digits, Narrowest>, IsSigned>
        : _impl::adopt_width<
                  wide_integer<Digits, set_signedness_t<Narrowest, IsSigned>>,
                  wide_integer<Digits, Narrowest>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_NUMBERS_H
