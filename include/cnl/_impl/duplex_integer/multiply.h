
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H)
#define CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H

#include "../custom_operator/definition.h"
#include "../custom_operator/op.h"
#include "../num_traits/width.h"
#include "../type_traits/conditional3.h"
#include "definition.h"
#include "digits.h"
#include "numeric_limits.h"
#include "set_width.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Lhs, typename Rhs>
        struct heterogeneous_duplex_multiply_operator {
            using common_type = conditional3_t<
                    width<Lhs> - width<Rhs>, Lhs,
                    conditional3_t<(numbers::signedness_v<Lhs> - numbers::signedness_v<Rhs>), Lhs, void, Rhs>,
                    Rhs>;

            [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            {
                return static_cast<common_type>(lhs) * static_cast<common_type>(rhs);
            }
        };

        // long_multiply - T should be same width as operands
        template<typename T>
        struct long_multiply;

        // std::int64_t
        template<typename Word>
        struct long_multiply {
            template<typename Lhs, typename Rhs>
            using result_type = set_width_t<Word, width<Lhs> + width<Rhs>>;

            template<typename Lhs, typename Rhs>
            [[nodiscard]] constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
                    -> result_type<Lhs, Rhs>
            {
                using product_type = result_type<Lhs, Rhs>;
                return static_cast<product_type>(lhs) * static_cast<product_type>(rhs);
            }
        };

        // duplex_integer<std::int64_t, std::int64_t>
        template<typename Upper, typename Lower>
        struct long_multiply<duplex_integer<Upper, Lower>> {
        private:
            using operand = duplex_integer<Upper, Lower>;
            using result_type =
                    duplex_integer<operand, duplex_integer<Lower, Lower>>;

        public:
            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            [[nodiscard]] constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs,
                    duplex_integer<RhsUpper, RhsLower> const& rhs) const -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
            }

            template<typename Lhs, typename RhsUpper, typename RhsLower>
            [[nodiscard]] constexpr auto operator()(
                    Lhs const& lhs, duplex_integer<RhsUpper, RhsLower> const& rhs) const
                    -> result_type
            {
                return multiply_components(0, lhs, rhs.upper(), rhs.lower());
            }

            template<typename LhsUpper, typename LhsLower, typename Rhs>
            [[nodiscard]] constexpr auto operator()(
                    duplex_integer<LhsUpper, LhsLower> const& lhs, Rhs const& rhs) const
                    -> result_type
            {
                return multiply_components(lhs.upper(), lhs.lower(), 0, rhs);
            }

            template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
            [[nodiscard]] static constexpr auto multiply_components(
                    LhsUpper const& lhs_upper, LhsLower const& lhs_lower, RhsUpper const& rhs_upper,
                    RhsLower const& rhs_lower) -> result_type
            {
                auto const upper_upper{_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper)};
                auto const upper_lower{_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)};
                auto const lower_upper{_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper)};
                auto const lower_lower{_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower)};
                auto const upper{_impl::sensible_left_shift<result_type>(
                        upper_upper,
                        digits_v<LhsLower> + digits_v<RhsLower>)};
                auto const mid{
                        (result_type{upper_lower} << digits_v<LhsLower>)+(result_type{lower_upper} << digits_v<RhsLower>)};
                auto const lower{lower_lower};
                return upper + mid + lower;
            }
        };
    }

    // duplex_integer<> * duplex_integer<>>
    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::multiply_op,
            op_value<_impl::duplex_integer<Upper, Lower>>,
            op_value<_impl::duplex_integer<Upper, Lower>>> {
    private:
        using operand = _impl::duplex_integer<Upper, Lower>;

    public:
        [[nodiscard]] constexpr auto operator()(
                operand const& lhs, operand const& rhs) const -> operand
        {
            return multiply_components(lhs.upper(), lhs.lower(), rhs.upper(), rhs.lower());
        }

        [[nodiscard]] static constexpr auto multiply_components(
                Upper const& lhs_upper, Lower const& lhs_lower, Upper const& rhs_upper,
                Lower const& rhs_lower) -> operand
        {
            auto const upper_upper{_impl::long_multiply<Upper>{}(lhs_upper, rhs_upper)};
            auto const upper_lower{_impl::long_multiply<Upper>{}(lhs_upper, rhs_lower)};
            auto const lower_upper{_impl::long_multiply<Upper>{}(lhs_lower, rhs_upper)};
            auto const lower_lower{_impl::long_multiply<Lower>{}(lhs_lower, rhs_lower)};
            auto const upper{_impl::sensible_left_shift<operand>(upper_upper, digits_v<Lower> * 2)};
            auto const mid{(upper_lower + lower_upper) << digits_v<Lower>};
            auto const lower{lower_lower};
            return upper + mid + lower;
        }
    };

    template<typename LhsUpper, typename LhsLower, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::multiply_op,
            op_value<_impl::duplex_integer<LhsUpper, LhsLower>>,
            op_value<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_multiply_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>,
                  _impl::duplex_integer<RhsUpper, RhsLower>> {
    };

    template<typename LhsUpper, typename LhsLower, typename Rhs>
    struct custom_operator<
            _impl::multiply_op,
            op_value<_impl::duplex_integer<LhsUpper, LhsLower>>,
            op_value<Rhs>>
        : _impl::heterogeneous_duplex_multiply_operator<
                  _impl::duplex_integer<LhsUpper, LhsLower>, Rhs> {
    };

    template<typename Lhs, typename RhsUpper, typename RhsLower>
    struct custom_operator<
            _impl::multiply_op,
            op_value<Lhs>,
            op_value<_impl::duplex_integer<RhsUpper, RhsLower>>>
        : _impl::heterogeneous_duplex_multiply_operator<
                  Lhs, _impl::duplex_integer<RhsUpper, RhsLower>> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_MULTIPLY_H
