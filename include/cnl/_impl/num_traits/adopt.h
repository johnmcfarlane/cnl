
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_ADOPT
#define CNL_IMPL_NUM_TRAITS_ADOPT

#include "adopt_digits.h"
#include "adopt_signedness.h"

namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt : adopt_digits<adopt_signedness_t<Mimic, Source>, Source> {
        };

        template<typename Mimic, typename Source>
        using adopt_t = typename adopt<Mimic, Source>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_ADOPT
