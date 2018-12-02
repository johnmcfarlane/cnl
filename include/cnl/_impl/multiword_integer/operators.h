
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_OPERATORS_H)
#define CNL_IMPL_MULTIWORD_INTEGER_OPERATORS_H 1

#include "../duplex_integer/operators.h"
#include "../num_traits/from_rep.h"
#include "../operators.h"
#include "forward_declaration.h"
#include "from_rep.h"
#include "from_value.h"
#include "numeric_limits.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename LhsWord, int LhsNumWords, class Rhs>
        constexpr auto operator<<(multiword_integer<LhsWord, LhsNumWords> const& lhs, Rhs const& rhs)
        -> multiword_integer<LhsWord, LhsNumWords>
        {
            return multiword_integer<LhsWord, LhsNumWords>(
                    to_rep(lhs) << set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }

        template<typename LhsWord, int LhsNumWords, class Rhs>
        constexpr auto operator>>(multiword_integer<LhsWord, LhsNumWords> const& lhs, Rhs const& rhs)
        -> multiword_integer<LhsWord, LhsNumWords>
        {
            return multiword_integer<LhsWord, LhsNumWords>(
                    to_rep(lhs) >> set_signedness_t<int, is_signed<Rhs>::value>(rhs));
        }

        template<class Operator, typename RhsWord, int RhsNumWords>
        struct unary_operator<Operator, multiword_integer<RhsWord, RhsNumWords>> {
            constexpr auto operator()(multiword_integer<RhsWord, RhsNumWords> const& rhs) const
            -> decltype(cnl::_impl::from_rep<multiword_integer<RhsWord, RhsNumWords>>(Operator{}(to_rep(rhs))))
            {
                return cnl::_impl::from_rep<multiword_integer<RhsWord, RhsNumWords>>(Operator{}(to_rep(rhs)));
            }
        };

        template<class Operator, typename Word, int NumWords>
        struct binary_operator<Operator, multiword_integer<Word, NumWords>, multiword_integer<Word, NumWords>> {
            constexpr auto operator()(
                    multiword_integer<Word, NumWords> const& lhs,
                    multiword_integer<Word, NumWords> const& rhs) const
            -> decltype(cnl::_impl::from_rep<multiword_integer<Word, NumWords>>(Operator{}(to_rep(lhs), to_rep(rhs))))
            {
                return cnl::_impl::from_rep<multiword_integer<Word, NumWords>>(Operator{}(to_rep(lhs), to_rep(rhs)));
            }
        };

        template<class Operator, typename Word, int NumWords>
        struct comparison_operator<Operator, multiword_integer<Word, NumWords>, multiword_integer<Word, NumWords>> {
            constexpr auto operator()(
                    multiword_integer<Word, NumWords> const& lhs, multiword_integer<Word, NumWords> const& rhs) const
            -> decltype(Operator{}(to_rep(lhs), to_rep(rhs)))
            {
                return Operator{}(to_rep(lhs), to_rep(rhs));
            }
        };
    }
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_OPERATORS_H
