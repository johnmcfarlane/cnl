
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_ADOPT_DIGITS_H)
#define CNL_IMPL_NUM_TRAITS_ADOPT_DIGITS_H

#include "../../cstdint.h"
#include "digits.h"
#include "set_digits.h"

namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt_digits : set_digits<Mimic, digits_v<Source>> {
        };

        template<typename Mimic, typename Source>
        using adopt_digits_t = typename adopt_digits<Mimic, Source>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_ADOPT_DIGITS_H
