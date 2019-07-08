
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_SHIFT_OPERATOR_H)
#define CNL_IMPL_INTEGER_SHIFT_OPERATOR_H

#include "from_rep.h"
#include "is_integer.h"
#include "operator_helpers.h"
#include "to_rep.h"
#include "../operators/native_tag.h"
#include "../type_traits/enable_if.h"

#include <ostream>

/// compositional numeric library
namespace cnl {
    template<class Operator, typename LhsRep, class LhsTag, class Rhs>
    struct shift_operator<
            _impl::native_tag, Operator, _impl::integer<LhsRep, LhsTag>, Rhs,
            _impl::enable_if_t<!_impl::is_same_integer_wrapper<_impl::integer<LhsRep, LhsTag>, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::integer<LhsRep, LhsTag> const& lhs, Rhs const& rhs) const
        -> decltype(_impl::integer_from_rep<LhsTag>(Operator()(_impl::to_rep(lhs), rhs)))
        {
            return _impl::integer_from_rep<LhsTag>(Operator()(_impl::to_rep(lhs), rhs));
        }
    };

    template<class Operator, class Lhs, class Rhs>
    struct shift_operator<
            _impl::native_tag, Operator, Lhs, Rhs,
            _impl::enable_if_t<
                    _impl::is_integer<Lhs>::value&&_impl::is_same_integer_wrapper<Lhs, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(_impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
        {
            return _impl::from_rep<Lhs>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::operator<<(std::ostream& o, cnl::_impl::integer const& i)

        template<class Rep, class Tag>
        std::ostream &operator<<(std::ostream &o, integer<Rep, Tag> const &i) {
            return o << to_rep(i);
        }
    }
}

#endif  // CNL_IMPL_INTEGER_SHIFT_OPERATOR_H
