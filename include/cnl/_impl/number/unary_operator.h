
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_UNARY_OPERATOR_H)
#define CNL_IMPL_NUMBER_UNARY_OPERATOR_H

#include "from_rep.h"
#include "make_number.h"
#include "to_rep.h"
#include "definition.h"
#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/overloads.h"

/// compositional numeric library
namespace cnl {
    template<typename Operator, typename Rep, class Tag>
    struct unary_operator<_impl::native_tag, Operator, _impl::number<Rep, Tag>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<Rep, Tag> const& rhs) const
        -> decltype(_impl::from_rep<_impl::number<Rep, Tag>>(unary_operator<Tag, Operator, Rep>{}(_impl::to_rep(rhs))))
        {
            return _impl::from_rep<_impl::number<Rep, Tag>>(unary_operator<Tag, Operator, Rep>{}(_impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_UNARY_OPERATOR_H
