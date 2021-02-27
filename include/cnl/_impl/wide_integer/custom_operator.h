
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_GENERIC_H)
#define CNL_IMPL_WIDE_INTEGER_GENERIC_H

#include "../num_traits/to_rep.h"
#include "../operators/custom_operator.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<
            _impl::comparison_op Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits,
            typename RhsNarrowest>
    requires(!std::is_same_v<
             wide_integer<LhsDigits, LhsNarrowest>,
             wide_integer<RhsDigits, RhsNarrowest>>) struct
            custom_operator<
                    Operator,
                    op_value<wide_integer<LhsDigits, LhsNarrowest>>,
                    op_value<wide_integer<RhsDigits, RhsNarrowest>>> {
        using result_tag = _impl::native_tag;

        [[nodiscard]] constexpr auto operator()(
                wide_integer<LhsDigits, LhsNarrowest> const& lhs,
                wide_integer<RhsDigits, RhsNarrowest> const& rhs) const
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_GENERIC_H
