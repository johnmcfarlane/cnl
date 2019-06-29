
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the integer rounding

#if !defined(CNL_IMPL_ROUNDING_H)
#define CNL_IMPL_ROUNDING_H

#include "native_tag.h"
#include "rounding/native_rounding_tag.h"
#include "rounding/nearest_rounding_tag.h"
#include "rounding/convert_operator.h"
#include "operators.h"
#include "num_traits/from_value.h"
#include "num_traits/unwrap.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::tagged_binary_operator<nearest_rounding_tag>

    template<class Operator>
    struct tagged_binary_operator<native_rounding_tag, Operator>
            : tagged_binary_operator<_impl::native_tag, Operator> {
    };

    template<class Operator>
    struct tagged_binary_operator<nearest_rounding_tag, Operator> : Operator {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::tagged_binary_operator<nearest_rounding_tag, divide_op>

    template<>
    struct tagged_binary_operator<nearest_rounding_tag, _impl::divide_op> {
        template<class Lhs, class Rhs>
        CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
        -> decltype(lhs/rhs)
        {
            return (((lhs < 0) ^ (rhs < 0))
                    ? lhs-(rhs/2)
                    : lhs+(rhs/2))/rhs;
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::divide<nearest_rounding_tag, ...>

        template<class RoundingTag, class Lhs, class Rhs>
        struct divide {
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs/rhs)
            {
                return tagged_binary_operator<RoundingTag, divide_op>{}(cnl::unwrap(lhs), cnl::unwrap(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::shift_right

        template<class RoundingTag, class Lhs, class Rhs>
        struct shift_right {
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs >> rhs)
            {
                return tagged_binary_operator<RoundingTag, shift_right_op>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_ROUNDING_H
