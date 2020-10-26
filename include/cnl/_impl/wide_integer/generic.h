
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_GENERIC_H)
#define CNL_IMPL_WIDE_INTEGER_GENERIC_H

#include "../num_traits/to_rep.h"
#include "../operators/generic.h"
#include "../type_traits/enable_if.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<class Operator, int LhsDigits, typename LhsNarrowest, int RhsDigits, typename RhsNarrowest>
    struct comparison_operator<
            Operator,
            wide_integer<LhsDigits, LhsNarrowest>,
            wide_integer<RhsDigits, RhsNarrowest>,
            _impl::enable_if_t<!std::is_same<
                    wide_integer<LhsDigits, LhsNarrowest>,
                    wide_integer<RhsDigits, RhsNarrowest>>::value>> {
        CNL_NODISCARD constexpr auto operator()(
                wide_integer<LhsDigits, LhsNarrowest> const& lhs,
                wide_integer<RhsDigits, RhsNarrowest> const& rhs) const
        {
            return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
        }
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_GENERIC_H
