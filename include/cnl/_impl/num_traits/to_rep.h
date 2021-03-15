
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_TO_REP_H)
#define CNL_IMPL_NUM_TRAITS_TO_REP_H

#include "../../constant.h"
#include "../type_traits/is_integral.h"
#include "../type_traits/remove_cvref.h"

#include <type_traits>
#include <utility>

namespace cnl {
    namespace _impl {
        template<typename Number>
        struct default_to_rep {
            [[nodiscard]] constexpr auto& operator()(Number& n) const
            {
                return n;
            };
            [[nodiscard]] constexpr auto const& operator()(Number const& n) const
            {
                return n;
            };
            [[nodiscard]] constexpr auto&& operator()(Number&& n) const
            {
                return std::forward<Number>(n);
            };
        };
    }

    /// \brief Returns the value encapsulated in \c number
    /// \param number the 'outer' object
    /// \return the 'inner' value
    /// \sa from_rep, from_value
    template<typename Number>
    struct to_rep;

    /// \cond
    template<typename Number>
    requires(_impl::is_integral_v<Number> || std::is_floating_point_v<Number> || _impl::is_constant<Number>::value) struct to_rep<Number>
        : _impl::default_to_rep<Number> {
    };
    /// \endcond

    namespace _impl {
        template<class Number>
        [[nodiscard]] constexpr auto to_rep(Number&& n)  // NOLINT(misc-unused-parameters)
                -> decltype(cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(n)))
        {
            return cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(n));
        }
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_TO_REP_H
