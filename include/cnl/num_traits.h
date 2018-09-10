
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting generic treatment of numeric types

#if !defined(CNL_NUM_TRAITS)
#define CNL_NUM_TRAITS 1

#include "_impl/num_traits/digits.h"
#include "_impl/num_traits/from_rep.h"
#include "_impl/num_traits/from_value.h"
#include "_impl/num_traits/is_composite.h"
#include "_impl/num_traits/fixed_width_scale.h"
#include "_impl/num_traits/set_digits.h"
#include "_impl/num_traits/scale.h"
#include "_impl/num_traits/to_rep.h"
#include "_impl/num_traits/unwrap.h"
#include "_impl/num_traits/wrap.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::is_integral

    using _impl::is_integral;
#if (__cplusplus > 201402L)
    using _impl::is_integral_v;
#endif
}

#endif  // CNL_NUM_TRAITS
