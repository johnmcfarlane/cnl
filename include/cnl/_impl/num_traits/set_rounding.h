
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_SET_ROUNDING)
#define CNL_IMPL_NUM_TRAITS_SET_ROUNDING

#include "../type_traits/enable_if.h"
#include "../type_traits/remove_cvref.h"
#include "../type_traits/type_identity.h"
#include "is_composite.h"
#include "rounding.h"

namespace cnl {
    /// \brief given a numeric type, defines member `type` as the equivalent type with the given rounding mode
    /// \headerfile cnl/num_traits.h
    /// \note User-specializations of this type are permitted.
    /// \note Native numeric types are only convertible to \ref cnl::native_rounding_tag.
    /// \sa cnl::rounding, cnl::set_rounding_t,
    /// cnl::native_rounding_tag, cnl::nearest_rounding_tag
    template<typename Number, class RoundingTag, class Enable = void>
    struct set_rounding;

    template<typename Number>
    struct set_rounding<Number, rounding_t<Number>, _impl::enable_if_t<!is_composite<Number>::value>>
            : _impl::type_identity<Number> {
    };

    template<typename Number, class RoundingTag>
    struct set_rounding<Number const&, RoundingTag>
            : set_rounding<_impl::remove_cvref_t<Number>, RoundingTag> {
    };

    template<typename Number, class RoundingTag>
    struct set_rounding<Number&, RoundingTag>
            : set_rounding<Number, RoundingTag> {
    };

    template<typename Number, class RoundingTag>
    struct set_rounding<Number&&, RoundingTag>
            : set_rounding<Number, RoundingTag> {
    };

    /// \brief helper alias of \ref cnl::set_rounding
    /// \headerfile cnl/num_traits.h
    /// \sa cnl::set_rounding, cnl::rounding_t,
    /// cnl::native_rounding_tag, cnl::nearest_rounding_tag
    template<typename Number, class RoundingTag>
    using set_rounding_t = typename set_rounding<Number, RoundingTag>::type;
}

#endif  // CNL_IMPL_NUM_TRAITS_SET_ROUNDING
