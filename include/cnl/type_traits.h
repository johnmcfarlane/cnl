
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/num_traits.h
/// \brief definitions supporting generic treatment of numeric types

#if !defined(CNL_TYPE_TRAITS_H)
#define CNL_TYPE_TRAITS_H

#include "_impl/type_traits/common_type.h"
#include "_impl/type_traits/enable_if.h"
#include "_impl/type_traits/is_integral.h"
#include "_impl/type_traits/remove_cvref.h"
#include "_impl/type_traits/type_identity.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    using _impl::common_type_t;

    using _impl::enable_if_t;

    using _impl::is_integral;
    using _impl::is_integral_v;

    using _impl::remove_cvref_t;

    using _impl::type_identity;
    using _impl::type_identity_t;
}

#endif  // CNL_TYPE_TRAITS_H
