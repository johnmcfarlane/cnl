
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_THROWING_H)
#define CNL_IMPL_OVERFLOW_THROWING_H

#include "common.h"
#include "overflow_operator.h"
#include "../throw_exception.h"

#include <stdexcept>

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify throw-on-overflow behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag throw \ref std::overflow_error when the result exceeds the range of the
    /// result type.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::trapping_overflow_tag, cnl::undefined_overflow_tag
    struct throwing_overflow_tag {
    };

    namespace _impl {
        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("positive overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>("positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, throwing_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return throw_exception<Destination, std::overflow_error>("negative overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const& ...) const
            {
                return throw_exception<op_result<Operator, Operands...>, std::overflow_error>("negative overflow");
            }
        };

        template<typename Result>
        struct negative_overflow_result<Result, throwing_overflow_tag> {
            CNL_NODISCARD constexpr Result operator()() const
            {
                return throw_exception<Result, std::overflow_error>("negative overflow");
            }
        };
    }

    template<typename Destination, typename Source>
    struct convert_operator<throwing_overflow_tag, Destination, Source>
            : _impl::tagged_convert_overflow_operator<throwing_overflow_tag, Destination, Source> {
    };

    template<class Operator>
    struct unary_operator<throwing_overflow_tag, Operator>
            : _impl::tagged_unary_overflow_operator<throwing_overflow_tag, Operator> {
    };

    template<class Operator>
    struct tagged_binary_operator<throwing_overflow_tag, Operator>
            : _impl::tagged_binary_overflow_operator<throwing_overflow_tag, Operator> {
    };
}

#endif  // CNL_IMPL_OVERFLOW_THROWING_H
