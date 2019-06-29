
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_TRAPPING_H)
#define CNL_IMPL_OVERFLOW_TRAPPING_H

#include "common.h"
#include "overflow_operator.h"
#include "../terminate.h"

/// compositional numeric library
namespace cnl {
    /// \brief tag to specify trap-on-overflow behavior in arithemtic operations
    ///
    /// Arithmetic operations using this tag call \ref std::terminate when the result exceeds the range of the result
    /// type.
    ///
    /// \headerfile cnl/overflow.h
    /// \sa cnl::overflow_integer,
    /// cnl::add, cnl::convert, cnl::divide, cnl::left_shift, cnl::multiply, cnl::subtract,
    /// cnl::native_overflow_tag, cnl::saturated_overflow_tag, cnl::throwing_overflow_tag, cnl::undefined_overflow_tag
    struct trapping_overflow_tag {
    };

    namespace _impl {
        template<typename Operator>
        struct overflow_operator<Operator, trapping_overflow_tag, polarity::positive> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return terminate<Destination>("positive overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const&...) const
            {
                return terminate<op_result<Operator, Operands...>>("positive overflow");
            }
        };

        template<typename Operator>
        struct overflow_operator<Operator, trapping_overflow_tag, polarity::negative> {
            template<typename Destination, typename Source>
            CNL_NODISCARD constexpr Destination operator()(Source const&) const
            {
                return terminate<Destination>("negative overflow");
            }

            template<class ... Operands>
            CNL_NODISCARD constexpr op_result<Operator, Operands...> operator()(Operands const&...) const
            {
                return terminate<op_result<Operator, Operands...>>("negative overflow");
            }
        };
    }

    template<typename Destination, typename Source>
    struct convert_operator<trapping_overflow_tag, Destination, Source>
            : _impl::tagged_convert_overflow_operator<trapping_overflow_tag, Destination, Source> {
    };

    template<class Operator>
    struct unary_operator<trapping_overflow_tag, Operator>
            : _impl::tagged_unary_overflow_operator<trapping_overflow_tag, Operator> {
    };

    template<class Operator>
    struct tagged_binary_operator<trapping_overflow_tag, Operator>
            : _impl::tagged_binary_overflow_operator<trapping_overflow_tag, Operator> {
    };
}

#endif  // CNL_IMPL_OVERFLOW_TRAPPING_H
