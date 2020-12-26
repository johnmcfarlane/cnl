
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_TO_REP_H)
#define CNL_IMPL_NUMBER_TO_REP_H

#include "../num_traits/rep.h"
#include "../num_traits/to_rep.h"
#include "../type_traits/enable_if.h"
#include "is_number.h"

/// compositional numeric library
namespace cnl {
    template<_impl::wrapper Number>
    struct to_rep<Number> {
        using rep_type = _impl::rep_t<Number>;
        CNL_NODISCARD constexpr rep_type& operator()(Number& number) const
        {
            return number._rep;
        }
        CNL_NODISCARD constexpr rep_type const& operator()(Number const& number) const
        {
            return number._rep;
        }
        CNL_NODISCARD constexpr rep_type&& operator()(Number&& number) const
        {
            return std::forward<rep_type>(number._rep);
        }
    };
}

#endif  // CNL_IMPL_NUMBER_TO_REP_H
