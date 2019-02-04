
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NUM_TRAITS_TO_REP
#define CNL_IMPL_NUM_TRAITS_TO_REP

#include "../type_traits/remove_cvref.h"
#include "../type_traits/enable_if.h"
#include "../type_traits/is_integral.h"
#include "../../constant.h"

#include <type_traits>
#include <utility>

namespace cnl {
    namespace _impl {
        template<typename Number>
        struct default_to_rep {
            constexpr Number& operator()(Number& number) const {
                return number;
            };
            constexpr Number const& operator()(Number const& number) const {
                return number;
            };
            constexpr Number&& operator()(Number&& number) const {
                return std::forward<Number>(number);
            };
        };
    }

    /// \brief Returns the value encapsulated in \c number
    /// \param number the 'outer' object
    /// \return the 'inner' value
    /// \sa from_rep, from_value
    template<typename Number, class Enable = void>
    struct to_rep;

    template<typename Number>
    struct to_rep<Number, _impl::enable_if_t<
            _impl::is_integral<Number>::value
            ||std::is_floating_point<Number>::value
            ||_impl::is_constant<Number>::value>>
            : _impl::default_to_rep<Number> {
    };

    namespace _impl {
        template<class Number>
        constexpr auto to_rep(Number&& number)
        -> decltype(cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(number))) {
            return cnl::to_rep<remove_cvref_t<Number>>()(std::forward<Number>(number));
        }

        template<class Number>
        using to_rep_t = typename std::remove_reference<decltype(to_rep(std::declval<Number>()))>::type;
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_TO_REP
