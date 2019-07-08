
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::overflow_integer` type

#if !defined(CNL_OVERFLOW_INT_H)
#define CNL_OVERFLOW_INT_H 1

#include "overflow.h"

#include "_impl/integer.h"
#include "_impl/number_base.h"
#include "_impl/ostream.h"
#include "_impl/num_traits/from_value.h"
#include "_impl/num_traits/from_value_recursive.h"
#include "_impl/operators/generic.h"
#include "_impl/operators/native_tag.h"
#include "_impl/operators/tagged.h"
#include "_impl/type_traits/common_type.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief An integer which detects overflow.
    template<typename Rep = int, class Tag = undefined_overflow_tag>
    using overflow_integer = _impl::integer<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::operator<<(cnl::_impl::integer, ...)

    template<class LhsRep, class LhsOverflowTag, typename Rhs>
    CNL_NODISCARD constexpr auto operator<<(_impl::integer<LhsRep, LhsOverflowTag> const& lhs, Rhs const& rhs)
            -> _impl::integer<_impl::op_result<_impl::shift_left_op, LhsRep, Rhs>, LhsOverflowTag>
    {
        return _impl::from_rep<_impl::integer<_impl::op_result<_impl::shift_left_op, LhsRep, Rhs>, LhsOverflowTag>>(
                binary_operator<LhsOverflowTag, _impl::shift_left_op, LhsRep, Rhs>{}(_impl::to_rep(lhs), rhs));
    }
}

#endif  // CNL_OVERFLOW_INT_H
