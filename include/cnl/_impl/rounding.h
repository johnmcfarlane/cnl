
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the integer rounding

#if !defined(CNL_IMPL_ROUNDING_H)
#define CNL_IMPL_ROUNDING_H

#include "num_traits/from_value.h"
#include "num_traits/unwrap.h"
#include "operators/native_tag.h"
#include "operators/operators.h"
#include "rounding/convert_operator.h"
#include "rounding/native_rounding_tag.h"
#include "rounding/nearest_rounding_tag.h"
#include "rounding/tie_to_pos_inf_rounding_tag.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::divide<nearest_rounding_tag, ...>

        template<class LhsTag, class RhsTag, class Lhs, class Rhs>
        struct divide {
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return cnl::binary_operator<divide_op, LhsTag, RhsTag, Lhs, Rhs>{}(
                        cnl::unwrap(lhs), cnl::unwrap(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::shift_right

        template<class LhsTag, class RhsTag, class Lhs, class Rhs>
        struct shift_right {
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return cnl::binary_operator<shift_right_op, LhsTag, RhsTag, Lhs, Rhs>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_ROUNDING_H
