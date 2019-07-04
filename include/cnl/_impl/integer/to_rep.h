
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_TO_REP_H)
#define CNL_IMPL_INTEGER_TO_REP_H

#include "type.h"
#include "../num_traits/to_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep>
    struct to_rep<_impl::integer<Rep>> {
        CNL_NODISCARD constexpr Rep& operator()(_impl::integer<Rep>& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep const& operator()(_impl::integer<Rep> const& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep&& operator()(_impl::integer<Rep>&& number) const {
            return std::forward<Rep>(number._rep);
        }
    };
}

#endif  // CNL_IMPL_INTEGER_TO_REP_H
