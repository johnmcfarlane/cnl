
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H)
#define CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H 1

#include "digits.h"
#include "numeric_limits.h"
#include "set_width.h"
#include "type.h"
#include "wants_generic_ops.h"
#include "../num_traits/width.h"
#include "../operators.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // long_multiply - T should be same width as operands
        template<typename T>
        struct long_multiply;

        // int64
        template<typename Word>
        struct long_multiply {
            template<typename Lhs, typename Rhs>
            using result_type = set_width_t<Word, width<Lhs>::value+width<Rhs>::value>;

            template<typename Lhs, typename Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type<Lhs, Rhs>
            {
                using result_type = result_type<Lhs, Rhs>;
                return static_cast<result_type>(lhs)*static_cast<result_type>(rhs);
            }
        };

        // duplex_integer<int64, int64>
        template<typename Upper, typename Lower>
        struct long_multiply<duplex_integer<Upper, Lower>> {
            using result_type = duplex_integer<duplex_integer<Upper, Lower>, duplex_integer<Lower, Lower>>;

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            template<typename Lhs, typename RhsUpper, typename RhsLower>
            constexpr auto operator()(
                    Lhs const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(0, lhs, rhs.upper(), rhs.lower());
            }

            template<typename LhsUpper, typename LhsLower, typename Rhs>
            constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    Rhs const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), 0, rhs);
            }

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            static constexpr auto multiply_components(
                    LhsUpper const& lhs_upper, LhsLower const& lhs_lower,
                    RhsUpper const& rhs_upper, RhsLower const& rhs_lower)
            -> result_type
            {
                return ((result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper)})
                        << (digits<Lower>::value+digits<Upper>::value))
                        +((result_type{_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper)}
                                +result_type{_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)})
                                << digits<Lower>::value)
                        +((result_type{_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower)}));
            }
        };

        template<typename Upper, typename Lower>
        struct binary_operator<multiply_op, duplex_integer<Upper, Lower>, duplex_integer<Upper, Lower>> {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
            -> _duplex_integer
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            static constexpr auto multiply_components(
                    Upper const& lhs_upper,
                    Lower const& lhs_lower,
                    Upper const& rhs_upper,
                    Lower const& rhs_lower)
            -> _duplex_integer
            {
                using common_result_type = decltype(long_multiply<Upper>{}(lhs_upper, rhs_upper));
                return (long_multiply<Upper>{}(lhs_upper, rhs_upper) << digits<Upper>::value)
                        +((long_multiply<Upper>{}(lhs_upper, rhs_lower)+long_multiply<Upper>{}(lhs_lower, rhs_upper))
                                << digits<Lower>::value)
                        +static_cast<common_result_type>(long_multiply<Lower>{}(lhs_lower, rhs_lower));
            }
        };
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H
