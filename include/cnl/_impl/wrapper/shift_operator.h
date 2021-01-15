
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_SHIFT_OPERATOR_H)
#define CNL_IMPL_WRAPPER_SHIFT_OPERATOR_H

#include "../operators/native_tag.h"
#include "../type_traits/enable_if.h"
#include "from_rep.h"
#include "is_wrapper.h"
#include "operator_helpers.h"
#include "tag_of.h"
#include "to_rep.h"

#include <ostream>

/// compositional numeric library
namespace cnl {
    // number << non-number
    template<_impl::shift_op Operator, typename LhsRep, tag LhsTag, class Rhs>
    requires _impl::number_can_wrap<_impl::wrapper<LhsRep, LhsTag>, Rhs>::value struct custom_operator<
            Operator,
            operand<_impl::wrapper<LhsRep, LhsTag>>,
            operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::wrapper<LhsRep, LhsTag> const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<_impl::wrapper<LhsRep, LhsTag>>(
                    custom_operator<Operator, operand<LhsRep, LhsTag>, operand<Rhs>>{}(
                            _impl::to_rep(lhs), rhs));
        }
    };

    // number<int, Foo> << number<int, Foo>
    // includes derived classes
    template<_impl::shift_op Operator, typename LhsRep, tag LhsTag, _impl::wrapped Rhs>
    struct custom_operator<
            Operator, operand<_impl::wrapper<LhsRep, LhsTag>>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::wrapper<LhsRep, LhsTag> const& lhs, Rhs const& rhs) const
        {
            return _impl::from_rep<_impl::wrapper<LhsRep, LhsTag>>(
                    custom_operator<
                            Operator, operand<LhsRep, LhsTag>, operand<_impl::rep_of_t<Rhs>>>{}(
                            _impl::to_rep(lhs), _impl::to_rep(rhs)));
        }
    };

    // non-number << number
    // includes derived classes
    template<_impl::shift_op Operator, class Lhs, _impl::wrapped Rhs>
    requires(!_impl::is_wrapper<Lhs>) struct custom_operator<Operator, operand<Lhs>, operand<Rhs>> {
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        {
            return Operator()(lhs, _impl::rep_of_t<Rhs>{_impl::to_rep(rhs)});
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::operator<<(std::ostream& o, cnl::_impl::wrapper const& i)

        template<class Rep, tag Tag>
        std::ostream& operator<<(std::ostream& o, wrapper<Rep, Tag> const& i)
        {
            return o << to_rep(i);
        }
    }
}

#endif  // CNL_IMPL_WRAPPER_SHIFT_OPERATOR_H
