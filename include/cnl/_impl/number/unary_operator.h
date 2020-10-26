
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_UNARY_OPERATOR_H)
#define CNL_IMPL_NUMBER_UNARY_OPERATOR_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "../operators/overloads.h"
#include "definition.h"
#include "from_rep.h"
#include "make_number.h"
#include "to_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Operator, typename Rep, class Tag>
    struct unary_operator<Operator, _impl::native_tag, _impl::number<Rep, Tag>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<Rep, Tag> const& rhs) const
        {
            return _impl::from_rep<_impl::number<Rep, Tag>>(unary_operator<Operator, Tag, Rep>{}(_impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_UNARY_OPERATOR_H
