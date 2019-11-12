
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_FROM_VALUE_RECURSIVE)
#define CNL_IMPL_NUM_TRAITS_FROM_VALUE_RECURSIVE

#include "../config.h"
#include "from_rep.h"
#include "from_value.h"
#include "rep.h"

namespace cnl {
    namespace _impl {
        template<class Number, typename Value>
        struct from_value_recursive {
            CNL_NODISCARD constexpr auto operator()(Value const& value) const
            -> decltype(cnl::_impl::from_rep<Number>(cnl::_impl::from_value<rep_t<Number>>(value)))
            {
                return value;
            }
        };
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_FROM_VALUE_RECURSIVE
