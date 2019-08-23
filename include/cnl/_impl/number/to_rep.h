
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TO_REP_H)
#define CNL_IMPL_NUMBER_TO_REP_H

#include "declaration.h"
#include "../num_traits/to_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, class Tag>
    struct to_rep<_impl::number<Rep, Tag>> {
        CNL_NODISCARD constexpr Rep& operator()(_impl::number<Rep, Tag>& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep const& operator()(_impl::number<Rep, Tag> const& number) const {
            return number._rep;
        }
        CNL_NODISCARD constexpr Rep&& operator()(_impl::number<Rep, Tag>&& number) const {
            return std::forward<Rep>(number._rep);
        }
    };
}

#endif  // CNL_IMPL_NUMBER_TO_REP_H
