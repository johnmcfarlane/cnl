
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_ADOPT_SIGNEDNESS
#define CNL_IMPL_NUM_TRAITS_ADOPT_SIGNEDNESS

#include "../type_traits/is_signed.h"
#include "../type_traits/set_signedness.h"

namespace cnl {
    namespace _impl {
        template<typename Mimic, typename Source>
        struct adopt_signedness : set_signedness<Mimic, is_signed<Source>::value> {
        };

        template<typename Mimic, typename Source>
        using adopt_signedness_t = typename adopt_signedness<Mimic, Source>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_ADOPT_SIGNEDNESS
