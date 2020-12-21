
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_ROUNDING_H)
#define CNL_IMPL_NUMBER_ROUNDING_H

#include "../num_traits/rep.h"
#include "../num_traits/rounding.h"
#include "../num_traits/tag.h"
#include "../type_traits/enable_if.h"
#include "is_number.h"

/// compositional numeric library
namespace cnl {
    template<class Number>
    struct rounding<
            Number, _impl::enable_if_t<
                            _impl::is_number<Number>::value
                            && !_impl::is_rounding_tag<_impl::tag_t<Number>>::value>>
        : rounding<_impl::rep_t<Number>> {
    };
}

#endif  // CNL_IMPL_NUMBER_ROUNDING_H
