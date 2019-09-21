
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

#include <ostream>

/// compositional numeric library
namespace cnl {
    template<class Operator, typename LhsRep, class LhsTag, class Rhs>
    struct shift_operator<
            Operator, _impl::native_tag, _impl::native_tag, _impl::number<LhsRep, LhsTag>, Rhs,
            _impl::enable_if_t<!_impl::is_same_number_wrapper<_impl::number<LhsRep, LhsTag>, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<LhsRep, LhsTag> const& lhs, Rhs const& rhs) const
        -> decltype(_impl::from_rep<_impl::number<LhsRep, LhsTag>>(Operator()(_impl::to_rep(lhs), rhs)))
        {
            return _impl::from_rep<_impl::number<LhsRep, LhsTag>>(Operator()(_impl::to_rep(lhs), rhs));
        }
    };

    template<class Operator, class LhsRep, class LhsTag, class Rhs>
    struct shift_operator<
            Operator, _impl::native_tag, _impl::native_tag, _impl::number<LhsRep, LhsTag>, Rhs,
            _impl::enable_if_t<
                    _impl::is_same_number_wrapper<_impl::number<LhsRep, LhsTag>, Rhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<LhsRep, LhsTag> const& lhs, Rhs const& rhs) const
        -> decltype(_impl::from_rep<_impl::number<LhsRep, LhsTag>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
        {
            return _impl::from_rep<_impl::number<LhsRep, LhsTag>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };

    template<class Operator, class Lhs, typename RhsRep, class RhsTag>
    struct shift_operator<
            Operator, _impl::native_tag, _impl::native_tag, Lhs, _impl::number<RhsRep, RhsTag>,
            _impl::enable_if_t<!_impl::is_number<Lhs>::value>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, _impl::number<RhsRep, RhsTag> const& rhs) const
        -> decltype(Operator()(lhs, RhsRep{_impl::to_rep(rhs)}))
        {
            return Operator()(lhs, RhsRep{_impl::to_rep(rhs)});
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::operator<<(std::ostream& o, cnl::_impl::number const& i)

        template<class Rep, class Tag>
        std::ostream &operator<<(std::ostream &o, number<Rep, Tag> const &i) {
            return o << to_rep(i);
        }
    }
}

#endif  // CNL_IMPL_NUMBER_SHIFT_OPERATOR_H
