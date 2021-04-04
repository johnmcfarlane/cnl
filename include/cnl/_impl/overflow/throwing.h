
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
namespace cnl::_impl {
    /// \brief tag to specify throw-on-overflow behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag throw \ref std::overflow_error when the result exceeds
    /// the range of the result type.
    ///
    /// \headerfile cnl/overflow.h
    /// \note This tag is defined only to test the versatility of \ref overflow_integer.
    /// Arithmetic overflow is a result of bugs. In C++, bugs should not be handled with exceptions.
    /// See Section 4.2 of [P0709](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0709r4.pdf)
    /// for more information.
    /// \sa overflow_integer, convert, native_overflow_tag, saturated_overflow_tag,
    /// trapping_overflow_tag, undefined_overflow_tag
    struct throwing_overflow_tag
        : _impl::homogeneous_deduction_tag_base
        , _impl::homogeneous_operator_tag_base {
    };

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

#endif  // CNL_IMPL_OVERFLOW_THROWING_H
