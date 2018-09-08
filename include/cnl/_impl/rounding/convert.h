
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_CONVERT_H)
#define CNL_IMPL_ROUNDING_CONVERT_H

#include "../native_tag.h"
#include "../type_traits/enable_if.h"
#include "rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<typename Result>
    struct convert<_impl::native_rounding_tag, Result> : convert<_impl::native_tag, Result> {
    };

    template<typename Result>
    struct convert<_impl::nearest_rounding_tag, Result> {
        template<typename Input>
        constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from+((from>=0) ? .5 : -.5));
        }
    };
}

#endif  // CNL_IMPL_ROUNDING_CONVERT_H
