
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_THROWING_H)
#define CNL_IMPL_OVERFLOW_THROWING_H

#include "../polarity.h"
#include "../throw_exception.h"
#include "is_overflow_tag.h"
#include "is_tag.h"
#include "overflow_operator.h"

#include <stdexcept>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify throw-on-overflow behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag throw \ref std::overflow_error when the result exceeds
    /// the range of the result type.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::trapping_overflow_tag,
    /// cnl::undefined_overflow_tag
    struct throwing_overflow_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

    namespace _impl {
        template<>
        struct is_overflow_tag<throwing_overflow_tag> : std::true_type {
        };

        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            [[nodiscard]] constexpr auto operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("positive overflow");
            }

            template<class... Operands>
            [[nodiscard]] constexpr auto operator()(Operands const&...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>(
                        "positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            [[nodiscard]] constexpr auto operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("negative overflow");
            }

            template<class... Operands>
            [[nodiscard]] constexpr auto operator()(Operands const&...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>(
                        "negative overflow");
            }
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_THROWING_H
