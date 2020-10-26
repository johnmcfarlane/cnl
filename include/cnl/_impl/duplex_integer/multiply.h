
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H)
#define CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H

#include "../num_traits/width.h"
#include "../operators/generic.h"
#include "../operators/operators.h"
#include "../type_traits/conditional3.h"
#include "digits.h"
#include "numeric_limits.h"
#include "set_width.h"
#include "type.h"
#include "wants_generic_ops.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_multiply_operator {
            using common_type = conditional3_t<
                    width<Lhs>::value-width<Rhs>::value,
                    Lhs,
                    conditional3_t<
                            (is_signed<Lhs>::value-is_signed<Rhs>::value),
                            Lhs,
                            void,
                            Rhs>,
                    Rhs>;

            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return static_cast<common_type>(lhs)*static_cast<common_type>(rhs);
            }
        };

        // long_multiply - T should be same width as operands
        template<typename T>
        struct long_multiply;

        // int64
        template<typename Word>
        struct long_multiply {
            template<typename Lhs, typename Rhs>
            using result_type = set_width_t<Word, width<Lhs>::value+width<Rhs>::value>;

            template<typename Lhs, typename Rhs>
            CNL_NODISCARD constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const -> result_type<Lhs, Rhs>
            {
                using product_type = result_type<Lhs, Rhs>;
                return static_cast<product_type>(lhs)*static_cast<product_type>(rhs);
            }
        };

        // duplex_integer<int64, int64>
        template<typename Upper, typename Lower>
        struct long_multiply<duplex_integer<Upper, Lower>> {
            using result_type = duplex_integer<duplex_integer<Upper, Lower>, duplex_integer<Lower, Lower>>;

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            CNL_NODISCARD constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            template<typename Lhs, typename RhsUpper, typename RhsLower>
            CNL_NODISCARD constexpr auto operator()(
                    Lhs const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const
            -> result_type
            {
                return multiply_components(0, lhs, rhs.upper(), rhs.lower());
            }

            template<typename LhsUpper, typename LhsLower, typename Rhs>
            CNL_NODISCARD constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    Rhs const& rhs) const
            -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), 0, rhs);
            }

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            CNL_NODISCARD static constexpr auto multiply_components(
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
    }

    // cnl::_impl::binary_operator<multiply_op, duplex_integer<>, duplex_integer<>>
    template<typename Upper, typename Lower>
    struct binary_operator<
            _impl::multiply_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<Upper, Lower>, _impl::duplex_integer<Upper, Lower>> {
        using _duplex_integer = _impl::duplex_integer<Upper, Lower>;

        CNL_NODISCARD constexpr auto operator()(_duplex_integer const& lhs, _duplex_integer const& rhs) const
        -> _duplex_integer
        {
            return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
        }

        CNL_NODISCARD static constexpr auto multiply_components(
                Upper const& lhs_upper,
                Lower const& lhs_lower,
                Upper const& rhs_upper,
                Lower const& rhs_lower)
        -> _duplex_integer
        {
            using common_result_type = decltype(_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper));
            return (_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper) << digits<Upper>::value)
                    +((_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)+_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper))
                            << digits<Lower>::value)
                    +static_cast<common_result_type>(_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower));
        }
    };

    template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
    struct binary_operator<
            _impl::multiply_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<LhsUpper, LhsLower>, _impl::duplex_integer<RhsUpper, RhsLower>>
            : _impl::heterogeneous_duplex_multiply_operator<
                    _impl::duplex_integer<LhsUpper, LhsLower>, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename LhsUpper, typename LhsLower, typename Rhs>
    struct binary_operator<
            _impl::multiply_op,
            _impl::native_tag, _impl::native_tag,
            _impl::duplex_integer<LhsUpper, LhsLower>, Rhs>
            : _impl::heterogeneous_duplex_multiply_operator<
                    _impl::duplex_integer<LhsUpper, LhsLower>, Rhs> {
    };

    template<typename Lhs, typename RhsUpper, typename RhsLower>
    struct binary_operator<
            _impl::multiply_op,
            _impl::native_tag, _impl::native_tag,
            Lhs, _impl::duplex_integer<RhsUpper, RhsLower>>
            : _impl::heterogeneous_duplex_multiply_operator<
                    Lhs, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H
