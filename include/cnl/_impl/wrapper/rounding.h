
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_ROUNDING_H)
#define CNL_IMPL_WRAPPER_ROUNDING_H

#include "../num_traits/rep_of.h"
#include "../num_traits/rounding.h"
#include "../num_traits/tag_of.h"
#include "is_wrapper.h"

/// compositional numeric library
namespace cnl {
    /// \cond
    template<_impl::wrapped Number>
    requires(!_impl::is_rounding_tag<_impl::tag_of_t<Number>>::value) struct rounding<Number>
        : rounding<_impl::rep_of_t<Number>> {
    };
    /// \endcond
}

#endif  // CNL_IMPL_WRAPPER_ROUNDING_H
