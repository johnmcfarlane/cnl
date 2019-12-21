
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H)
#define CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H

#include "../operators/generic.h"
#include "../operators/native_tag.h"
#include "definition.h"
#include "to_rep.h"

namespace cnl {
    template<class Operator, typename Rep, class Tag>
    struct pre_operator<_impl::native_tag, Operator, _impl::number<Rep, Tag>> {
        CNL_RELAXED_CONSTEXPR _impl::number<Rep, Tag>& operator()(_impl::number<Rep, Tag>& rhs) const
        {
            pre_operator<Tag, Operator, Rep>{}(_impl::to_rep(rhs));
            return rhs;
        }
    };

    template<class Operator, typename Rep, class Tag>
    struct post_operator<_impl::native_tag, Operator, _impl::number<Rep, Tag>> {
        CNL_RELAXED_CONSTEXPR _impl::number<Rep, Tag> operator()(_impl::number<Rep, Tag>& lhs) const
        {
            return post_operator<Tag, Operator, Rep>{}(_impl::to_rep(lhs));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_INC_DEC_OPERATOR_H
