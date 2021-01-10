
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H

#include "../../wide_integer.h"
#include "../common.h"
#include "../num_traits/width.h"
#include "../operators/custom_operator.h"
#include "../operators/operators.h"
#include "../to_chars.h"
#include "../type_traits/common_type.h"
#include "add_signedness.h"
#include "comparison.h"
#include "ctors.h"
#include "digits.h"
#include "divide.h"
#include "modulo.h"
#include "multiply.h"
#include "numeric_limits.h"
#include "set_digits.h"
#include "shift.h"
#include "type.h"

#include <ostream>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // default binary arithmetic operator
        template<binary_arithmetic_op Operator, typename Upper, typename Lower>
        struct default_binary_arithmetic_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            CNL_NODISCARD constexpr auto operator()(
                    _duplex_integer const& lhs, _duplex_integer const& rhs) const -> _duplex_integer
            {
                return _duplex_integer(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        static_cast<Lower>(Operator{}(lhs.lower(), rhs.lower())));
            }
        };

        // add_op and subtract_op
        template<binary_arithmetic_op Operator, typename Upper, typename Lower>
        struct first_degree_binary_arithmetic_operator {
            using _duplex_integer = duplex_integer<Upper, Lower>;

            static constexpr auto lower_digits = digits<Lower>;
            using wide_lower = set_digits_t<set_signedness_t<Lower, true>, lower_digits + 1>;

            CNL_NODISCARD constexpr auto operator()(
                    _duplex_integer const& lhs, _duplex_integer const& rhs) const -> _duplex_integer
            {
                return from_sums(
                        static_cast<Upper>(Operator{}(lhs.upper(), rhs.upper())),
                        wide_lower(Operator{}(wide_lower{lhs.lower()}, wide_lower{rhs.lower()})));
            }

            CNL_NODISCARD static constexpr auto from_sums(
                    Upper const& upper_sum, wide_lower const& lower_sum) -> _duplex_integer
            {
                return _duplex_integer{
                        static_cast<Upper>(
                                upper_sum
                                + static_cast<Upper>(lower_sum >> constant<lower_digits>{})),
                        static_cast<Lower>(lower_sum)};
            }
        };
    }

    // unary
    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::bitwise_not_op, operand<_impl::duplex_integer<Upper, Lower>>> {
        CNL_NODISCARD constexpr auto operator()(_impl::duplex_integer<Upper, Lower> const& rhs)
                const -> _impl::duplex_integer<Upper, Lower>
        {
            return _impl::duplex_integer<Upper, Lower>(~rhs.upper(), ~rhs.lower());
        }
    };

    // binary arithmetic
    template<typename Upper, typename Lower>
    struct custom_operator<_impl::minus_op, operand<_impl::duplex_integer<Upper, Lower>>> {
        CNL_NODISCARD constexpr auto operator()(_impl::duplex_integer<Upper, Lower> const& rhs)
                const -> _impl::duplex_integer<Upper, Lower>
        {
            return custom_operator<
                    _impl::bitwise_not_op,
                    operand<_impl::duplex_integer<Upper, Lower>>>{}(
                    rhs - _impl::duplex_integer<Upper, Lower>{1});
        }
    };

    template<typename Upper, typename Lower>
    struct custom_operator<_impl::plus_op, operand<_impl::duplex_integer<Upper, Lower>>> {
        CNL_NODISCARD constexpr auto operator()(_impl::duplex_integer<Upper, Lower> const& rhs)
                const -> _impl::duplex_integer<Upper, Lower>
        {
            return _impl::duplex_integer<Upper, Lower>(+rhs.upper(), +rhs.lower());
        }
    };

    template<_impl::binary_arithmetic_op Operator, typename Upper, typename Lower, typename Rhs>
    struct custom_operator<
            Operator,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<Rhs>>
        : custom_operator<
                  Operator,
                  operand<_impl::duplex_integer<Upper, Lower>>,
                  operand<_impl::duplex_integer<Upper, Lower>>> {
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::add_op,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>>
        : _impl::first_degree_binary_arithmetic_operator<_impl::add_op, Upper, Lower> {
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::subtract_op,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>>
        : _impl::first_degree_binary_arithmetic_operator<_impl::subtract_op, Upper, Lower> {
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::bitwise_or_op,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>>
        : _impl::default_binary_arithmetic_operator<_impl::bitwise_or_op, Upper, Lower> {
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::bitwise_and_op,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>>
        : _impl::default_binary_arithmetic_operator<_impl::bitwise_and_op, Upper, Lower> {
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::bitwise_xor_op,
            operand<_impl::duplex_integer<Upper, Lower>>,
            operand<_impl::duplex_integer<Upper, Lower>>>
        : _impl::default_binary_arithmetic_operator<_impl::bitwise_xor_op, Upper, Lower> {
    };

    template<
            _impl::comparison_op Operator, typename LhsUpper, typename LhsLower, typename RhsUpper,
            typename RhsLower>
    struct custom_operator<
            Operator,
            operand<_impl::duplex_integer<LhsUpper, LhsLower>>,
            operand<_impl::duplex_integer<RhsUpper, RhsLower>>> {
        CNL_NODISCARD constexpr auto operator()(
                _impl::duplex_integer<LhsUpper, LhsLower> const& lhs,
                _impl::duplex_integer<RhsUpper, RhsLower> const& rhs) const -> bool
        {
            using common_type = _impl::duplex_integer<
                    _impl::common_type_t<LhsUpper, RhsUpper>,
                    _impl::common_type_t<LhsLower, RhsLower>>;
            return custom_operator<Operator, operand<common_type>, operand<common_type>>{}(lhs, rhs);
        }
    };

    // prefix operators
    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::pre_increment_op, operand<_impl::duplex_integer<Upper, Lower>>> {
        CNL_NODISCARD constexpr auto operator()(_impl::duplex_integer<Upper, Lower>& rhs) const
                -> _impl::duplex_integer<Upper, Lower>
        {
            return CNL_UNLIKELY(rhs.lower() == numeric_limits<Lower>::max())
                         ? _impl::duplex_integer<
                                 Upper, Lower>{++rhs.upper(), numeric_limits<Lower>::lowest()}
                         : _impl::duplex_integer<Upper, Lower>{rhs.upper(), ++rhs.lower()};
        }
    };

    template<typename Upper, typename Lower>
    struct custom_operator<
            _impl::pre_decrement_op, operand<_impl::duplex_integer<Upper, Lower>>> {
        CNL_NODISCARD constexpr auto operator()(_impl::duplex_integer<Upper, Lower>& rhs) const
                -> _impl::duplex_integer<Upper, Lower>
        {
            return CNL_UNLIKELY(rhs.lower() == numeric_limits<Lower>::lowest())
                         ? _impl::duplex_integer<
                                 Upper,
                                 Lower>{static_cast<Upper>(--rhs.upper()), numeric_limits<Lower>::max()}
                         : _impl::duplex_integer<Upper, Lower>{rhs.upper(), --rhs.lower()};
        }
    };

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::duplex_integer streaming

        template<typename Upper, typename Lower>
        ::std::ostream& operator<<(::std::ostream& out, duplex_integer<Upper, Lower> const& value)
        {
            return out << cnl::to_chars(value).data();
        }
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_OPERATORS_H
