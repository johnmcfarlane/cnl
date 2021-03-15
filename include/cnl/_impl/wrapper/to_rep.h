
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_TO_REP_H)
#define CNL_IMPL_WRAPPER_TO_REP_H

#include "../num_traits/rep_of.h"
#include "../num_traits/to_rep.h"
#include "is_wrapper.h"

/// compositional numeric library
namespace cnl {
    template<_impl::any_wrapper Number>
    struct to_rep<Number> {
        using rep_type = _impl::rep_of_t<Number>;

        [[nodiscard]] constexpr auto& operator()(Number& n) const
        {
            return n._rep;
        }

        [[nodiscard]] constexpr auto const& operator()(Number const& n) const
        {
            return n._rep;
        }

        [[nodiscard]] constexpr auto&& operator()(Number&& n) const
        {
            return std::forward<rep_type>(n._rep);
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_TO_REP_H
