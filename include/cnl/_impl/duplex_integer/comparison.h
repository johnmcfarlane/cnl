
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H)
#define CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H 1

#include "is_duplex_integer.h"
#include "numeric_limits.h"
#include "type.h"
#include "wants_generic_ops.h"
#include "../operators.h"
#include "../type_traits/common_type.h"

#include <tuple>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Operator, typename Upper, typename Lower>
        struct comparison_operator<Operator, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            constexpr auto operator()(
                    duplex_integer<Upper, Lower> const& lhs,
                    duplex_integer<Upper, Lower> const& rhs) const -> bool
            {
                using tuple = std::tuple<Upper const&, Lower const&>;
                return Operator{}(tuple(lhs.upper(), lhs.lower()), tuple(rhs.upper(), rhs.lower()));
            }
        };

        template<class Operator, typename Lhs, typename Rhs>
        struct comparison_operator<Operator, Lhs, Rhs,
                enable_if_t<is_duplex_integer<Lhs>::value!=is_duplex_integer<Rhs>::value>> {
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> bool
            {
                using common_type = common_type_t<Lhs, Rhs>;
                return comparison_operator<Operator, common_type, common_type>{}(lhs, rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_COMPARISON_H
