
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_SET_ROUNDING_H)
#define CNL_IMPL_WRAPPER_SET_ROUNDING_H

#include "../num_traits/rep_of.h"
#include "../num_traits/set_rounding.h"
#include "../num_traits/set_tag.h"
#include "../num_traits/tag_of.h"
#include "../rounding/is_rounding_tag.h"
#include "../type_traits/enable_if.h"
#include "is_wrapper.h"

/// compositional numeric library
namespace cnl {
    template<_impl::wrapped Number, rounding_tag RoundingTag>
    struct set_rounding<
            Number, RoundingTag,
            _impl::enable_if_t<!_impl::is_rounding_tag<_impl::tag_of_t<Number>>::value>>
        : set_rep<Number, set_rounding_t<_impl::rep_of_t<Number>, RoundingTag>> {
    };

    template<_impl::wrapped Number, rounding_tag RoundingTag>
    struct set_rounding<
            Number, RoundingTag,
            _impl::enable_if_t<_impl::is_rounding_tag<_impl::tag_of_t<Number>>::value>>
        : set_tag<Number, RoundingTag> {
    };
}

#endif  // CNL_IMPL_WRAPPER_SET_ROUNDING_H
