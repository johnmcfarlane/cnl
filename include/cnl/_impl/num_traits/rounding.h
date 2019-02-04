
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting \ref cnl::rounding

#ifndef CNL_IMPL_NUM_TRAITS_ROUNDING
#define CNL_IMPL_NUM_TRAITS_ROUNDING

#include "../rounding/rounding_tag.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/type_identity.h"

namespace cnl {
    // cnl::native_rounding_tag
    using _impl::native_rounding_tag;

    // cnl::nearest_rounding_tag
    using _impl::nearest_rounding_tag;

    // cnl::rounding
    template<typename Number, class Enable = void>
    struct rounding;

    template<typename Number>
    struct rounding<Number&&> : rounding<Number> {};

    template<typename Number>
    struct rounding<Number, _impl::enable_if_t<_impl::is_integral<Number>::value>>
            : _impl::type_identity<native_rounding_tag> {
    };

    // cnl::rounding_t
    template<typename Number>
    using rounding_t = typename rounding<Number>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_ROUNDING
