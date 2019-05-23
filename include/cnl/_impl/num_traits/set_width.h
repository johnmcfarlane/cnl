
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::set_digits

#ifndef CNL_IMPL_NUM_TRAITS_SET_WIDTH
#define CNL_IMPL_NUM_TRAITS_SET_WIDTH

#include "set_digits.h"
#include "../type_traits/is_signed.h"

namespace cnl {
    namespace _impl {
        template<class T, int Width>
        struct set_width : set_digits<T, Width - is_signed<T>::value> {
        };

        template<class T, int Width>
        using set_width_t = typename set_width<T, Width>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_WIDTH
