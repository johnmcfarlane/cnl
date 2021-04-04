
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_ROUNDING_H)
#define CNL_IMPL_NUM_TRAITS_ROUNDING_H

#include "../rounding/native_rounding_tag.h"
#include "../rounding/nearest_rounding_tag.h"
#include "../type_traits/is_integral.h"

#include <type_traits>

namespace cnl {
    /// \brief given a numeric type, defines member `type` as its rounding mode
    /// \headerfile cnl/num_traits.h
    /// \note User-specializations of this type are permitted.
    /// \sa set_rounding, rounding_t, native_rounding_tag, nearest_rounding_tag
    template<typename Number>
    struct rounding;

    template<typename Number>
    struct rounding<Number&&> : rounding<Number> {
    };

    template<_impl::integral Number>
    struct rounding<Number>
        : std::type_identity<native_rounding_tag> {
    };

    /// \brief helper alias of \ref rounding
    /// \headerfile cnl/num_traits.h
    /// \sa rounding, set_rounding_t, native_rounding_tag, nearest_rounding_tag
    template<typename Number>
    using rounding_t = typename rounding<Number>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_ROUNDING_H
