
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_BITS_NUM_TRAITS_FOR_REP
#define CNL_BITS_NUM_TRAITS_FOR_REP

#include "is_composite.h"
#include "to_rep.h"
#include <cnl/bits/type_traits.h>

#include <utility>

namespace cnl {
    namespace _impl {
        // invoke a given generic lambda on given parameters
        template<class Result, class F, class ... Args,
                _impl::enable_if_t<!_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return f(std::forward<Args>(args)...);
        }

        template<class Result, class F, class ... Args,
                _impl::enable_if_t<_impl::are_composite<Args ...>::value, int> dummy = 0>
        constexpr Result for_rep(F f, Args &&...args) {
            return for_rep<Result>(f, _impl::to_rep(std::forward<Args>(args))...);
        }
    }
}

#endif  // CNL_BITS_NUM_TRAITS_FOR_REP
