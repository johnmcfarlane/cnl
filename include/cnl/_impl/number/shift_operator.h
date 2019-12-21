
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SHIFT_OPERATOR_H)
#define CNL_IMPL_NUMBER_SHIFT_OPERATOR_H

#include "../operators/native_tag.h"
#include "../type_traits/enable_if.h"
#include "from_rep.h"
#include "is_number.h"
#include "operator_helpers.h"
#include "to_rep.h"

/// compositional numeric library
namespace cnl {
    template<class Operator, class Lhs, class Rhs>
    struct shift_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<
                    _impl::is_number<Lhs>::value&&!_impl::is_same_number_wrapper<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(_impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), rhs)))
        {
            return _impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), rhs));
        }
    };

    template<class Operator, class Lhs, class Rhs>
    struct shift_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<
                    _impl::is_number<Lhs>::value&&_impl::is_same_number_wrapper<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(_impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
        {
            return _impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };
}

#endif  // CNL_IMPL_NUMBER_SHIFT_OPERATOR_H
